/** $lic$
 * Copyright (C) 2012-2015 by Massachusetts Institute of Technology
 * Copyright (C) 2010-2013 by The Board of Trustees of Stanford University
 *
 * This file is part of zsim.
 *
 * zsim is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, version 2.
 *
 * If you use this software in your research, we request that you reference
 * the zsim paper ("ZSim: Fast and Accurate Microarchitectural Simulation of
 * Thousand-Core Systems", Sanchez and Kozyrakis, ISCA-40, June 2013) as the
 * source of the simulator in any publications that use this software, and that
 * you send us a citation of your work.
 *
 * zsim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

//#include "timing_event.h"
//#include "event_recorder.h"
#include "mem_ctrls.h"
#include "zsim.h"
#include "simple_core.h"

class FilterCache;

uint64_t SimpleMemory::access(MemReq& req) {
    switch (req.type) {
        case PUTS:
        case PUTX:
            *req.state = I;
            break;
        case GETS:
            *req.state = req.is(MemReq::NOEXCL)? S : E;
            break;
        case GETX:
            *req.state = M;
            break;

        default: panic("!?");
    }
	
	

    uint64_t respCycle = req.cycle + latency;
    assert(respCycle > req.cycle);
/*
    if ((req.type == GETS || req.type == GETX) && eventRecorders[req.srcId]) {
        Address addr = req.lineAddr<<lineBits;
        MemAccReqEvent* memEv = new (eventRecorders[req.srcId]->alloc<MemAccReqEvent>()) MemAccReqEvent(nullptr, false, addr);
        TimingRecord tr = {addr, req.cycle, respCycle, req.type, memEv, memEv};
        eventRecorders[req.srcId]->pushRecord(tr);
    }
*/
    return respCycle;
}





MD1Memory::MD1Memory(uint32_t requestSize, uint32_t megacyclesPerSecond, uint32_t megabytesPerSecond, uint32_t _zeroLoadLatency, g_string& _name)
    : zeroLoadLatency(_zeroLoadLatency), name(_name)
{
    lastPhase = 0;

    double bytesPerCycle = ((double)megabytesPerSecond)/((double)megacyclesPerSecond);
    maxRequestsPerCycle = bytesPerCycle/requestSize;
    assert(maxRequestsPerCycle > 0.0);

    zeroLoadLatency = _zeroLoadLatency;

    smoothedPhaseAccesses = 0.0;
    curPhaseAccesses = 0;
    curLatency = zeroLoadLatency;

    futex_init(&updateLock);
}

void MD1Memory::updateLatency() {
    uint32_t phaseCycles = (zinfo->numPhases - lastPhase)*(zinfo->phaseLength);
    if (phaseCycles < 10000) return; //Skip with short phases

    smoothedPhaseAccesses =  (curPhaseAccesses*0.5) + (smoothedPhaseAccesses*0.5);
    double requestsPerCycle = smoothedPhaseAccesses/((double)phaseCycles);
    double load = requestsPerCycle/maxRequestsPerCycle;

    //Clamp load
    if (load > 0.95) {
        //warn("MC: Load exceeds limit, %f, clamping, curPhaseAccesses %d, smoothed %f, phase %ld", load, curPhaseAccesses, smoothedPhaseAccesses, zinfo->numPhases);
        load = 0.95;
        profClampedLoads.inc();
    }

    double latMultiplier = 1.0 + 0.5*load/(1.0 - load); //See Pollancek-Khinchine formula
    curLatency = (uint32_t)(latMultiplier*zeroLoadLatency);

    //info("%s: Load %.2f, latency multiplier %.2f, latency %d", name.c_str(), load, latMultiplier, curLatency);
    uint32_t intLoad = (uint32_t)(load*100.0);
    profLoad.inc(intLoad);
    profUpdates.inc();

    curPhaseAccesses = 0;
    __sync_synchronize();
    lastPhase = zinfo->numPhases;
}

uint64_t MD1Memory::access(MemReq& req) {
    if (zinfo->numPhases > lastPhase) {
        futex_lock(&updateLock);
        //Recheck, someone may have updated already
        if (zinfo->numPhases > lastPhase) {
            updateLatency();
        }
        futex_unlock(&updateLock);
    }

    switch (req.type) {
        case PUTX:
            //Dirty wback
            profWrites.atomicInc();
            profTotalWrLat.atomicInc(curLatency);
            __sync_fetch_and_add(&curPhaseAccesses, 1);
            //Note no break
        case PUTS:
            //Not a real access -- memory must treat clean wbacks as if they never happened.
            *req.state = I;
            break;
        case GETS:
            profReads.atomicInc();
            profTotalRdLat.atomicInc(curLatency);
            __sync_fetch_and_add(&curPhaseAccesses, 1);
            *req.state = req.is(MemReq::NOEXCL)? S : E;
            break;
        case GETX:
            profReads.atomicInc();
            profTotalRdLat.atomicInc(curLatency);
            __sync_fetch_and_add(&curPhaseAccesses, 1);
            *req.state = M;
            break;

        default: panic("!?");
    }
    return req.cycle + ((req.type == PUTS)? 0 /*PUTS is not a real access*/ : curLatency);
}
void SimplePIMMemory::initStats(AggregateStat* parentStat) {
	AggregateStat* memStats = new AggregateStat();
	memStats->init(name.c_str(), "Simple PIMMemory controller stats");
	flushedReq.init("fr", "flushed requests"); memStats->append(&flushedReq);
	flushedRead.init("frd", "flushed read requests"); memStats->append(&flushedRead);
	flushedWrite.init("frw", "flushed write requests"); memStats->append(&flushedWrite);
	flushedTotalDelay.init("ftd", "flushed requests delay"); memStats->append(&flushedTotalDelay);
	parentStat->append(memStats);
}
uint64_t SimplePIMMemory::access(MemReq& req) {
	
	if (!zinfo->pim) panic("use PIM memory in non-PIM system");
	
	
	
	uint64_t flushDelay = 0;
	
	switch (req.srcId)
	{
	case 0:
		if (SimpleCore* core = dynamic_cast <SimpleCore*>(zinfo->cores[1])) {
			//	((core->l1d)->array)->lookup(req.lineAddr, &req, false);
			bool reqWriteback = true;
			InvReq flushReq = { req.lineAddr, INV, &reqWriteback, req.cycle, req.srcId };
			uint64_t l1iDelay = 0;
			uint64_t l1dDelay = 0;
			//core->flush(flushReq);
			
			bool l1i = core->tryInvalidate(flushReq, true);
			bool l1d = core->tryInvalidate(flushReq, false);
			if (l1i) {
				l1iDelay = core->flush(flushReq, true);
				flushedReq.inc();
				if (req.type == PUTS || req.type == PUTX)
				{
					flushedWrite.inc();
				}
				else
				{
					if (req.type == GETS || req.type == GETX)
					{
						flushedRead.inc();
					}
				}
			}
			if (l1d) {
				l1dDelay = core->flush(flushReq, false);
				flushedReq.inc();
				if (req.type == GETS || req.type == GETX)
				{
					flushedRead.inc();
				}
				else
				{
					
					if (req.type == PUTS || req.type == PUTX)
					{
						flushedWrite.inc();
					}
				}
			}
			
			flushDelay += max(l1iDelay, l1dDelay);
		
		}
		else {
			panic("!?");
		}
		break;
	case 1:
		if (SimpleCore* core = dynamic_cast <SimpleCore*>(zinfo->cores[0])) {
			//	((core->l1d)->array)->lookup(req.lineAddr, &req, false);
			bool reqWriteback = true;
			InvReq flushReq = { req.lineAddr, INV, &reqWriteback, req.cycle, req.srcId };
			uint64_t l1iDelay = 0;
			uint64_t l1dDelay = 0;
			uint64_t l2Delay = 0;
			uint64_t l3Delay = 0;
			//core->flush(flushReq);
			bool l3 = core->tryInvalidate(flushReq);
			if (l3) {
				l3Delay = core->flush(flushReq);
				flushedReq.inc();
				if (req.type == PUTS || req.type == PUTX)
				{
					flushedWrite.inc();
				}
				else
				{
					if (req.type == GETS || req.type == GETX)
					{
						flushedRead.inc();
					}
					
				}
			}
			bool l2 = core->tryInvalidate(flushReq, 2);
			if (l2) {
				l2Delay = core->flush(flushReq, 2);
				flushedReq.inc();
				if (req.type == PUTS || req.type == PUTX)
				{
					flushedWrite.inc();
				}
				else
				{
					if (req.type == GETS || req.type == GETX)
					{
						flushedRead.inc();
					}
					
				}
			}
			bool l1i = core->tryInvalidate(flushReq, 0);
			bool l1d = core->tryInvalidate(flushReq, 1);
			if (l1i) {
				l1iDelay = core->flush(flushReq, 0);
				flushedReq.inc();
				if (req.type == PUTS || req.type == PUTX)
				{
					flushedWrite.inc();
				}
				else
				{
					if (req.type == GETS || req.type == GETX)
					{
						flushedRead.inc();
					}
				}
			}
			if (l1d) {
				l1dDelay = core->flush(flushReq, 1);
				flushedReq.inc();
				if (req.type == GETS || req.type == GETX)
				{
					flushedRead.inc();
				}
				else
				{
					
					if (req.type == PUTS || req.type == PUTX)
					{
						flushedWrite.inc();
					}
				}
			}
			
			flushDelay += max(max(max(l1iDelay, l1dDelay), l2Delay),l3Delay);
		
		}
		else {
			panic("!?");
		}
	
		break;
	default:
		panic("!?");
	}
	
	switch (req.type) {
	case PUTS:
	case PUTX:
		*req.state = I;
		break;
	case GETS:
		*req.state = req.is(MemReq::NOEXCL) ? S : E;
		break;
	case GETX:
		*req.state = M;
		break;

	default: panic("!?");
	}
	uint64_t respCycle = req.cycle + (req.srcId ==0? latency:latency*0.65) + flushDelay;
	//uint64_t respCycle = req.cycle + latency + flushDelay;
	flushedTotalDelay.inc(flushDelay);
	assert(respCycle > req.cycle);
	/*
	    if ((req.type == GETS || req.type == GETX) && eventRecorders[req.srcId]) {
	        Address addr = req.lineAddr<<lineBits;
	        MemAccReqEvent* memEv = new (eventRecorders[req.srcId]->alloc<MemAccReqEvent>()) MemAccReqEvent(nullptr, false, addr);
	        TimingRecord tr = {addr, req.cycle, respCycle, req.type, memEv, memEv};
	        eventRecorders[req.srcId]->pushRecord(tr);
	    }
	    */
	return respCycle;
}