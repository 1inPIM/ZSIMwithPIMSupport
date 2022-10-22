
#ifndef HMC_H_
#define HMC_H_
#include <vector>
#include <tuple>


#include <iostream>
#include <utility>
#include <string>
#include <bitset>
#include <map>
#include "galloc.h"
#define  BLOCK32
//#define  BLOCK64
//#define  BLOCK128
class GlobAlloc;
class Bank {
public:
   
	uint64_t Read = 0;
	uint64_t Write = 0;
	uint64_t PIM_Read = 0;
	uint64_t PIM_Write = 0;
  
	// std::pair<uint64_t, uint64_t> range;
	 //Bank(uint64_t a, uint64_t b){range=std::make_pair(a, b);}
	 Bank()
		: Read(0)
		, Write(0)
		, PIM_Read(0)
		, PIM_Write (0) {}
	void Access(uint64_t addr, bool write) {
		if (write)
			Write++;
		else
			Read++;
	}
	void PIMAccess(uint64_t addr, bool write) {
		if (write)
			PIM_Write++;
		else
			PIM_Read++;
	}
};
class TSV {
public:
	Bank bank[8];
	TSV() {
		//bank = new Bank[2 * 4];
	}
};
class Vault {
public:
	TSV tsv;
	uint64_t PIMNeed[16];
	uint64_t PIMCount;
	Vault()
	{
		//tsv = new TSV();
		for (int i = 0; i < 16; i++)
			PIMNeed[i] = 0;
		PIMCount = 0;
	}
		
};
//#define com_dblp_ungraph_r_txt
//#define Slashdot0811_r_txt
#define soc_LiveJournal1_r_txt
//#define WikiTalk_r_txt
//#define p2p_Gnutella31_r_txt
//#define com_youtube_ungraph_r_txt
//#define web_Google_r
//#define Wiki_Vote_r

//#define p2p_Gnutella04_r
//#define  p2p_Gnutella05_r
//#define  p2p_Gnutella06_r
//#define  p2p_Gnutella08_r
//#define  p2p_Gnutella09_r



#ifdef web_Google_r
#define NODES 916428
#endif
#ifdef com_dblp_ungraph_r_txt
#define NODES 425887
#endif
#ifdef Slashdot0811_r_txt
#define NODES 77360
#endif
#ifdef soc_LiveJournal1_r_txt
#define NODES 4847571
#endif
#ifdef WikiTalk_r_txt
#define NODES 2394385
#endif
#ifdef p2p_Gnutella31_r_txt
#define NODES 62586
#endif
#ifdef com_youtube_ungraph_r_txt
#define NODES 1157807
#endif
#ifdef Wiki_Vote_r
#define NODES 8298
#endif


#ifdef p2p_Gnutella04_r
#define NODES 10876
#endif
#ifdef p2p_Gnutella05_r
#define NODES 8846
#endif
#ifdef p2p_Gnutella06_r
#define NODES 8717
#endif

#ifdef p2p_Gnutella08_r
#define NODES 6301
#endif
#ifdef p2p_Gnutella09_r
#define NODES 8114
#endif
class LAD {
public:
	uint64_t lastload = 0;
	uint64_t laststore = 0;
	uint64_t lasttotal = 0;
	uint64_t loadcount = 0;
	uint64_t storecount = 0;
	uint64_t total_load = 0.0;
	uint64_t total_store = 0.0;
	
	uint64_t maxtotal = 0;
	uint64_t mintotal = 0xFFFFFFFFFFFFFFF;
	
	
	uint64_t maxload = 0;
	uint64_t maxstore = 0;
	uint64_t minload = 0xFFFFFFFFFFFFFFF;
	uint64_t minstore = 0xFFFFFFFFFFFFFFF;
	uint64_t total_dis = 0.0;
	LAD()
	{}
	;
	
}
;

class HMC
{
public:
    
	Vault vault[16];
	uint64_t current = 0;
	std::map<uint64_t, LAD> dis; 
	uint64_t count = 0;
	HMC()
	{
		//vault = new Vault[16];
		dis.clear();
	}
	void messureDistance(uint64_t addr, bool load=true)
	{
		uint64_t block_addr = addr >> 6;
		if (dis.find(block_addr) == dis.end())
		{
			
			LAD toadd;
			if (load)
			{
				toadd.lastload = count;
				toadd.loadcount++;
			}
			else
			{
				toadd.laststore = count;
				toadd.storecount++;
			}
			toadd.lasttotal = count;
			dis.insert(std::map<uint64_t, LAD>::value_type(block_addr, toadd));
		}
		else
		{
			//found
			if((count - dis[block_addr].lasttotal) > dis[block_addr].maxtotal)
					dis[block_addr].maxtotal = count - dis[block_addr].lasttotal;
			if ((count - dis[block_addr].lasttotal) != 0 && (count - dis[block_addr].lasttotal) < dis[block_addr].mintotal)
				dis[block_addr].mintotal = count - dis[block_addr].lasttotal;
			dis[block_addr].total_dis += count - dis[block_addr].lasttotal;
			dis[block_addr].lasttotal = count;
			if(load) {
				if ((count - dis[block_addr].lastload) > dis[block_addr].maxload)
					dis[block_addr].maxload = count - dis[block_addr].lastload;
				if ((count - dis[block_addr].lastload) != 0 && (count - dis[block_addr].lastload) < dis[block_addr].minload)
					dis[block_addr].minload = count - dis[block_addr].lastload;
				dis[block_addr].total_load += count - dis[block_addr].lastload;
				//dis[block_addr].total_dis += count - dis[block_addr].lastload;
				dis[block_addr].loadcount++;
				dis[block_addr].lastload = count;
			}else
			{
				if ((count - dis[block_addr].laststore) > dis[block_addr].maxstore)
					dis[block_addr].maxstore = count - dis[block_addr].laststore;
				if ((count - dis[block_addr].laststore) != 0 && (count - dis[block_addr].laststore) < dis[block_addr].minstore)
					dis[block_addr].minstore = count - dis[block_addr].laststore;
				dis[block_addr].total_store += count - dis[block_addr].laststore;
				//dis[block_addr].total_dis += count - dis[block_addr].laststore;
				dis[block_addr].storecount++;
				dis[block_addr].laststore = count;
			}
			
			
		}
		count++;
	}
		
		
		
	void setCurrent(uint64_t pim_addr)
	{
		assert(current == 0);
		
		current = pim_addr;
	}
	void resetCurrent()
	{
		assert(current != 0);
		
		current = 0;
	}
	void access(uint64_t addr, bool write) {
		vault[getVault(addr)].tsv.bank[getBank(addr)].Access(addr, write);
	}
	void PIMaccess(uint64_t addr, bool write) {
		vault[getVault(addr)].tsv.bank[getBank(addr)].PIMAccess(addr, write);
	}
	int getVault(uint64_t addr) {
		std::bitset<64> _addr(addr);
#ifdef BLOCK32
		return _addr[8] * 8 + _addr[7] * 4 + _addr[6] * 2 + _addr[5];
#else
#ifdef BLOCK64
		return _addr[9] * 8 + _addr[8] * 4 + _addr[7] * 2 + _addr[6];
#else
#ifdef BLOCK128
		return _addr[10] * 8 + _addr[9] * 4 + _addr[8] * 2 + _addr[7];
#endif // BLOCK128

#endif // BLOCK64

		
#endif // BLOCK32

		
	}
	int getBank(uint64_t addr) {
		std::bitset<64> _addr(addr);
#ifdef BLOCK32
		return _addr[11] * 4 + _addr[10] * 2 + _addr[9];
#else
#ifdef BLOCK64
		return _addr[12] * 4 + _addr[11] * 2 + _addr[10];
#else
#ifdef BLOCK128
		return _addr[13] * 4 + _addr[12] * 2 + _addr[11];
#endif // BLOCK128

#endif // BLOCK64

		
#endif // BLOCK32
		
	}
	void PIMNeed(uint64_t addr) {
		assert(current != 0);
		vault[getVault(current)].PIMNeed[getVault(addr)]++;
	}
	void PIM(uint64_t addr) {
		vault[getVault(addr)].PIMCount++;
	}
};
#endif
