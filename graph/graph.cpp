//
//  main.cpp
//  ReaderTest
//
//  Created by 徐晨 on 2017/12/12.
//  Copyright © 2017年 徐晨. All rights reserved.
//
#include "zsim_hooks.h"
#include <iostream>

#include <fstream>
#include <string.h>
#include <vector>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
//#include <queue>
#include <stdio.h>
#include <stack>
#include <iostream>
#include <time.h>
#include <thread>
#include <signal.h>
//#define MultiThread 6
//#define test



// 
// 
//#define p2p_Gnutella31_r_txt


//#define Wiki_Vote_r

//#define p2p_Gnutella04_r
//#define  p2p_Gnutella05_r
//#define  p2p_Gnutella06_r
//#define  p2p_Gnutella08_r
//#define  p2p_Gnutella09_r


#define p2p_Gnutella30_r
//#define Slashdot0811_r_txt
//#define Amazon0302_r
//#define com_dblp_ungraph_r_txt
//#define web_Google_r
//#define com_youtube_ungraph_r_txt
//#define WikiTalk_r_txt
//#define soc_LiveJournal1_r_txt


//#define sgraph_bellman_ford
#define sgraph_page_rank
//#define sgraph_teenage_follower
//#define sgraph_bfs
//#define sgraph_cc
//#define sgraph_kc
//#define sgraph_tc
//#define common




#ifdef web_Google_r
#define NODES 916428
#endif
#ifdef com_dblp_ungraph_r_txt
#define NODES 425957
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
#ifdef Amazon0302_r
#define NODES 262111
#endif
#ifdef p2p_Gnutella30_r
#define NODES 36682
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

using namespace std;


#define BFS_MAX_ITERATIONS 10000
#define ulong_t   unsigned long
#define PAGERANK_MAX_ITERATIONS 100
#define BFS_kCORE_ITERATIONS 10
//#define pointer_chasing
#define PAGERANK_MAX_ERROR 0.001
#define MAX_WEIGHT 5
enum Color { white, red, green }
;

typedef struct Node
{
	/* Average Teenage Follower */
//#ifdef sgraph_teenage_follower
	bool teenager;
	ulong_t followers;
//#endif
    
	/* PageRank */
#if (defined sgraph_page_rank)||(defined common)
	float page_rank;
	float next_rank;
#endif
    
	/* BFS */
#ifdef sgraph_bfs
	ulong_t distance;
	Color color;
#endif
#ifdef sgraph_cc
	 ulong_t distance;
	 Color color;
#endif
#ifdef sgraph_tc
	ulong_t distance;
	Color color;
#endif
#ifdef sgraph_kc
	ulong_t degree;
	ulong_t core;
#endif
	/* Bellman-ford */
#ifdef sgraph_bellman_ford
	ulong_t distance;
	ulong_t*  weights;             // List of weights (weighted graph)
#endif
    
	    /* General parameters */
	    ulong_t       ID;                  // ID of the current node
	    ulong_t       out_degree;          // Number of successor nodes
	    struct Node** successors;          // List of successors nodes
} node;
#define NC   ((ulong_t)(-1))
#define step 1

#define MAX_OUTDEGREE 10
node* nodes;                  // Graph nodes
ulong_t successors_size;      // Size of the successors list
node**  successors_list;      // List of the successors (not used directly)
//static int** adj;




#ifdef sgraph_teenage_follower

void reset_graph_stats()
{
	for (ulong_t i = 0; i < NODES; i++)
	{
		nodes[i].followers = 0;
	}
}
ulong_t run_golden()
{
	ulong_t r = 0;
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);


	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
	for (; r < NODES;)
	{
	//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
		
		
		if (nodes[r].teenager) {
			ulong_t c = 0;
		//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
			
			   
			for (; c < nodes[r].out_degree;)
			{
				
			//	
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
			
				node*succ = nodes[r].successors[c];
			//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
				
				
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
				zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
				zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
				succ->followers++;
				zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
				zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
			//	
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
				c++;
			}
		//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
			   

		}
		
	
		//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
		r++;
		
		//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
	}

	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);




	ulong_t total_followers = 0;
	r = 0;

	//	

		
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
	for (; r < NODES;) {
		//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
		zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
		//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
		total_followers += nodes[r].followers;
	//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
		zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
	//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
		r++;
	}
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
	
	
	return total_followers;
}




#endif


#ifdef sgraph_page_rank
void reset_graph_stats()
{
	for (ulong_t i = 0; i < NODES; i++)
	{
		nodes[i].page_rank = 0;
		nodes[i].next_rank = 0;
	}
}
#ifndef  MultiThread



ulong_t run_golden()
{
	//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
	ulong_t i = 0;
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
	 for( ; i < NODES ;)
	{
		//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
		zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
	//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
		nodes[i].page_rank = 1.0 / NODES;
		
		nodes[i].next_rank = 0.15 / NODES;
	//	
		//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
		zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
	//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO
		//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
		  i++;
		//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
	}
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
	//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
	//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
	ulong_t count = 0;	
	float diff = 0.0;
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
	do {
	//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
		ulong_t i = 0;
	//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
		for (; i < NODES; )	//4990
		{
		//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
			zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
			//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
			float delta = 0.85 * nodes[i].page_rank / nodes[i].out_degree;
		//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
			zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
			//	
			ulong_t j = 0;
		//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
		//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
			for (; j < nodes[i].out_degree; ) {
			//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
			    
				//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
					
				
				//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
				
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
				//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
				node * n = nodes[i].successors[j];
			//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
				
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
				zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
				//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
				n->next_rank += delta; //4993
			//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
			zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
			//	
				
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
			//	
				 j++;
			}
			//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
		//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
		//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
			i++;
		}
	//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
		 
		diff = 0.0;//4997
		i = 0;
		//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
		for (; i < NODES; ) //4998
		{
			//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
		//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
			zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
		//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
			   diff += fabsf(nodes[i].next_rank - nodes[i].page_rank); //4999
			
			nodes[i].page_rank = nodes[i].next_rank;
		
			nodes[i].next_rank = 0.15 / NODES;//5000
		//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
			zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
			//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
		//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
			i++;
		}
		//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
		cout << "ITERATION: " << count << " ERROR: " << diff << endl;//5003
		//print_graph();
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
	} while (++count < PAGERANK_MAX_ITERATIONS && diff > PAGERANK_MAX_ERROR) ; //5007-5009
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
	//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
   return (ulong_t)(diff * 1000000.0);   // Convert error to fixed point integer//5010
}
#else
	void ParalllelFor(int j) {
		for (ulong_t i = j*(NODES / 16); i <= (j+1)*(NODES / 16); i++)
		{
			 
			float delta = 0.85 * nodes[i].page_rank / nodes[i].out_degree;
			
			for (ulong_t j = 0; j < nodes[i].out_degree; j++) {
		
			    
				zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
				nodes[i].successors[j]->next_rank += delta;
				zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
				 
			}
	
		}
	}
ulong_t run_golden(){

	
	//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
		
	for (ulong_t i = 0; i < NODES; i++)
	{
		 
		nodes[i].page_rank = 1.0 / NODES;
		 
		nodes[i].next_rank = 0.15 / NODES;
		 
	}
	
		ulong_t count = 0;
		float diff = 0.0;
	 
		do {
		 
			//			for(ulong_t i = 0 ; i < NODES ; i++)
			//			{
			//			 
			//				float delta = 0.85 * nodes[i].page_rank / nodes[i].out_degree;
			//			
			//				for(ulong_t j = 0 ; j < nodes[i].out_degree ; j++) {
			//		
			//			    
			//					zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
			//					nodes[i].successors[j]->next_rank += delta;
			//					zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
			//				 
			//				}
			//	
			//			}
			thread t1(ParalllelFor, 0);
			thread t2(ParalllelFor, 1);
			thread t3(ParalllelFor, 2);
			thread t4(ParalllelFor, 3);
			thread t5(ParalllelFor, 4);
			thread t6(ParalllelFor, 5);
			thread t7(ParalllelFor, 6);
			thread t8(ParalllelFor, 7);
			thread t9(ParalllelFor, 8);
			thread t10(ParalllelFor, 9);
			thread t11(ParalllelFor, 10);
			thread t12(ParalllelFor, 11);
			thread t13(ParalllelFor, 12);
			thread t14(ParalllelFor, 13);
			thread t15(ParalllelFor, 14);
			thread t16(ParalllelFor, 15);
			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
			t8.join();
			t9.join();
			t10.join();
			t11.join();
			t12.join();
			t13.join();
			t14.join();
			t15.join();
			t16.join();
			
		 
			diff = 0.0;
		 
			for(ulong_t i = 0 ; i < NODES ; i++)
			{
				//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
				   diff += fabsf(nodes[i].next_rank - nodes[i].page_rank);
			
				nodes[i].page_rank = nodes[i].next_rank;
		
				nodes[i].next_rank = 0.15 / NODES;
				//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
			}
		
			cout << "ITERATION: " << count << " ERROR: " << diff << endl;
			//print_graph();
	
		} while (++count < PAGERANK_MAX_ITERATIONS && diff > PAGERANK_MAX_ERROR) ;
		//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
	   return (ulong_t)(diff * 1000000.0);  // Convert error to fixed point integer
}
	
#endif // MultiThread

#endif
#ifdef sgraph_bfs
ulong_t* queue;     // queue for BFS search
    ulong_t  head, tail, elements;
#define queue_init     { head=0; tail=0; elements=0; }
#define queue_push(X)  { queue[head]=(ulong_t)X;  head++;  if (head>=NODES) head=0; elements++; }
#define queue_pop      { tail++; if (tail>=NODES) tail=0;  elements--; } 
#define queue_top      ( queue[tail] )
#define queue_empty    ( elements==0 )
#define queue_full     ( elements==(NODES-1) )

//#define NC   ((ulong_t)(-1))

void reset_graph_stats()
{
	for (ulong_t i = 1; i < NODES; i++)
	{
		nodes[i].distance = NC;
	}
	nodes[0].distance = 0;        // node[0] is the starting node
	queue_init;                   // Initialize queue
}

void print_graph()
{
	for (ulong_t i = 0; i < NODES; i++)
		cout << "Node: " << i << " Distance:" << nodes[i].distance << endl;
}

ulong_t run_golden()
{
	// cout << "Visited 0 with distance 0" << endl;
	ulong_t total_distance = 0;
	int x = 0;
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
	//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
	
	for (; x < NODES; )
	{
	//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
	//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
		nodes[x].distance = 0;        // node[0] is the starting node
		queue_init;                   // Initialize queue
		queue_push(&nodes[x]);                // Push the first element
	//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
		while(!(queue_empty))
		{
		//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
			node* v = (node*)queue_top;
			queue_pop;
			ulong_t c = 0;
				
	////	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
		//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
		//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
			//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
			for (; c < v->out_degree; )
			{
			//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
				//
				zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
				zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
				node*succ = v->successors[c];
				zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
				zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
				
				if (succ->distance == NC) // Infinite
					{
					//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
					//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
						//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
						zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
						succ->distance = v->distance + 1;
						total_distance += succ->distance;
					//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
						zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
						// cout << "Visited " << succ->ID << " with distance " << succ->distance << endl;
					//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
					//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
						queue_push(succ);
					//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
					//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
					}
			////	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
				
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
			//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
				c++;
			}
			//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
		//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
		////	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
		}
		//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
	//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
		//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
	//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
		x++;
	}
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
	//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
	return total_distance;
}

//queue<ulong_t> node_queue;     // queue for BFS search
////ulong_t  head, tail, elements;
//
//void reset_graph_stats()
//{
//	for (ulong_t i = 0; i < NODES; i++)
//	{
//		nodes[i].distance = NC;
//		nodes[i].color = white;
//	}
//}
//
//ulong_t run_golden()
//{
//	// cout << "Visited 0 with distance 0" << endl;
//	ulong_t total_distance = 0;
//	//m5_switch2cache();
//	for(int x = 0 ; x < BFS_MAX_ITERATIONS ; x++)
//	{
//		 
//		  for(ulong_t i = 0 ; i < NODES ; i++)
//		{
//			 
//			 nodes[i].distance = NC;
//			//   m5_switch2cache();
//			   nodes[i].color = white;
//			 
//		}
//		//   m5_switch2cache();
//		   nodes[x].distance = 0;        // node[0] is the starting node
//		  
//		   nodes[x].color = green;   					   // queue_push(&nodes[x]);               // Push the first element
//		//   m5_switch2cache();
//		   node_queue.push(x);
//		//   m5_switch2cache();
//		   while(!node_queue.empty())
//		{
//			 
//			  node* v = &nodes[node_queue.front()];
//			//   m5_switch2cache();
//			   //queue_pop;
//			   node_queue.pop();
//			 
//			 for(ulong_t c = 0 ; c < v->out_degree ; c++)
//			{
//				 
//				  node* succ = v->successors[c];
//				//    m5_switch2cache();
//				    //if (succ.distance == NC) // Infinite
//				    if(succ->color == white)
//				{
//					 
//					  succ->distance = v->distance + 1;
//					//   m5_switch2cache();
//					   total_distance += succ->distance;
//					//    m5_switch2cache();
//				   //     cout << "Visited " << succ->ID << " with distance " << succ->distance << endl;
//				        //queue_push(succ);
//				        node_queue.push(succ->ID);
//					 
//					  succ->color = green;
//					//	cout << "push " << succ->ID;
//				   
//                    
//				}
//				 
//                
//			}
//			//   m5_switch2cache();
//			   v->color = red;
//			 
//		}
//	}
//	return total_distance;
//
//}

#endif

#ifdef sgraph_cc

queue<ulong_t> node_queue;     // queue for BFS search
//ulong_t  head, tail, elements;

void reset_graph_stats()
{
	for (ulong_t i = 0; i < NODES; i++)
	{
		// nodes[i].distance = NC;
		 nodes[i].color = white;
	}
}

ulong_t run_golden()
{
	// cout << "Visited 0 with distance 0" << endl;
	ulong_t total_cc = 0;
	//m5_switch2cache();
	for(int x = 0 ; x < NODES ; x++)
	{
		 
		  if(nodes[x].color != white) {
			//   m5_switch2cache();
			   continue;
			//   m5_switch2cache();
		}
		 
		  nodes[x].color = green;   					   // queue_push(&nodes[x]);               // Push the first element
		  //   m5_switch2cache();
		  node_queue.push(x);
		//   m5_switch2cache();
		while(!node_queue.empty())
		{
			 
			node* v = &nodes[node_queue.front()];
			//   m5_switch2cache();
			//queue_pop;
			node_queue.pop();
			 
			  for(ulong_t c = 0 ; c < v->out_degree ; c++)
			{
				 
				node* succ = v->successors[c];
				//    m5_switch2cache();
				//if (succ.distance == NC) // Infinite
				if(succ->color == white)
				{
					 
				  //  succ->distance = v->distance + 1;
				    //   m5_switch2cache();
				  //  total_distance += succ->distance;
				    //    m5_switch2cache();
				    //     cout << "Visited " << succ->ID << " with distance " << succ->distance << endl;
				    //queue_push(succ);
				    node_queue.push(succ->ID);
					 
					succ->color = green;
					//	cout << "push " << succ->ID;
					 
                    
				}
				 
                
			}
			 
			  v->color = red;
			 
		}
		//   m5_switch2cache();
		   total_cc++;
		//   m5_switch2cache();
	}
	return total_cc;
    
}

#endif

#ifdef sgraph_kc
void reset_graph_stats()
{
	for (ulong_t i = 0; i < NODES; i++)
	{
		nodes[i].degree = nodes[i].out_degree;  // Infinity
		nodes[i].core = 0;
	}
    
}

ulong_t run_golden()
{
	ulong_t checksum = 0;

	for (unsigned i = 1; i <= BFS_kCORE_ITERATIONS; i++) {
		for (unsigned r = 0; r < NODES; r++) {
			 
			 if(nodes[r].core != 0)
			     continue;
			 
			  if(nodes[r].degree > i)
			      continue;
			 
			  nodes[r].core = i;
			//    m5_switch2cache();
			    for(ulong_t c = 0 ; c < nodes[r].out_degree ; c++) // for node.successors
			{
				//     m5_switch2cache();
				     node* d = nodes[r].successors[c];
				//    m5_switch2cache();
				    d->degree--;
				//    m5_switch2cache();
			}
		}
	}
	return checksum;
}
#endif
#ifdef sgraph_tc

void reset_graph_stats()
{
	for (ulong_t i = 0; i < NODES; i++)
	{
		nodes[i].distance = 0;
		//nodes[i].color = white;
	}
	//nodes[0].distance = 0;       // node[0] is the starting node
	//queue_init;                  // Initialize queue
}
/*ulong_t run_golden()
{
    ulong_t dc = 0;
    m5_switch2cache();
    for ( unsigned r=0; r<NODES; r++ ){
        m5_switch2cache();
        for ( ulong_t c=0; c<nodes[r].out_degree; c++ ) // for node.successors
        {
            m5_switch2cache();
            //for ( unsigned g=0; g<NODES; g++ ){
            //      if(nodes[r].successors[c]->ID==g)
            //   {
            nodes[r].successors[c]->distance++;
            m5_switch2cache();
            dc++;
            m5_switch2cache();
            //     }
        }
    }
    

return dc;
}*/
ulong_t run_golden()
{
	ulong_t ret = 0;
    
	for (ulong_t i = 0; i < NODES; i++)
	{
		vector <ulong_t> src;
		src.push_back(i);
		for (ulong_t j = 0; j < nodes[i].out_degree; j++)
		{
			vector<ulong_t>::iterator it;
			for (it = src.begin(); it != src.end(); it++) {
				if (*it == nodes[i].successors[j]->ID) {
					break;
				}
			}
			if (it != src.end())
				continue;
			node* u = nodes[i].successors[j];
			src.push_back(nodes[i].successors[j]->ID);
			for (ulong_t k = 0; k < u->out_degree; k++)
			{
				vector<ulong_t>::iterator its;
				for (its = src.begin(); its != src.end(); its++) {
					if (*its == u->successors[k]->ID) {
						break;
					}
				}
				if (its != src.end())
					continue;

               
				ret++;
				nodes[u->successors[k]->ID].distance++;
				src.clear();
			}
		}
	}
	m5_switch2cache();
	for (ulong_t i = 0; i < NODES; i++)
	{
		m5_switch2cache();
		nodes[i].distance /= 2;
		m5_switch2cache();
		ret += nodes[i].distance;
		m5_switch2cache();
	}
	m5_switch2cache();
	ret /= 3;
	return ret;
}

#endif

#ifdef sgraph_bellman_ford

ulong_t  weights_size;      // Size of the weights list
ulong_t* weights_list;      // List of the weights (not used directly)

void reset_graph_stats()
{
	for (ulong_t i = 1; i < NODES; i++)
	{
		nodes[i].distance = NC;  // Infinity
	}
	nodes[0].distance = 0;        // node[0] is the starting node
}


ulong_t run_golden()
{
	ulong_t checksum = 0;
	ulong_t r = 0;
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
	for( ; r < NODES; ) {
	//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
		ulong_t c = 0;
	//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
	//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
		for (; c < nodes[r].out_degree; ) // for node.successors
			{
				//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
				node*u = &nodes[r];
				//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
		//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
				node*v = nodes[r].successors[c];
				//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
				zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
				zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
				ulong_t w = nodes[r].weights[c];
				zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
				if (u->distance != NC && v->distance > u->distance + w) {
					
					v->distance = u->distance + w;
					
				}
			//	
			zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
				checksum += w;
				
				
			//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
			//	zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
				c++;
			}
		//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
	//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
	//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
		r++;
	}
//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
	return checksum;
}

#endif


#ifdef common
void reset_graph_stats()
{
	for (ulong_t i = 0; i < NODES; i++)
	{
		nodes[i].page_rank = 0;
		nodes[i].next_rank = 0;
	}
}
ulong_t run_golden()
{

	for (ulong_t i = 0; i < NODES; i++)
	{
		nodes[i].page_rank = 1.0 / NODES;
		nodes[i].next_rank = 0.15 / NODES;
	}
	ulong_t count = 0;
	 
    
	  for(ulong_t i = 0 ; i < NODES ; i++)
	{
		//   m5_switch2cache();
		   Node* iter = &nodes[i];
		m5_switch2cache();
		while (iter->successors != NULL) {
			m5_switch2cache();
			iter = iter->successors[0];
			m5_switch2cache();
			count++;
			m5_switch2cache();
		}
	}
	//m5_switch2cache();
	return 1;  // Convert error to fixed point integer
}
#endif
//void create_graph()
//{
//	// Initialize the list of lists
//	// Later, we should read the graph from data sets
//	nodes = (node*)malloc(NODES*sizeof(node));
//#ifdef sgraph_bfs
//	//   queue = (ulong_t*)malloc(NODES*sizeof(ulong));
//#endif
//	    successors_size = 0;
//	successors_list = NULL;
//	//cout<<"1";
//    
//	//  cout<<"1";
//	for(ulong_t i = 0 ; i < NODES ; i++)
//	{
//        
//		ulong_t num_succ = (i % step == 0) ? (rand() % MAX_OUTDEGREE) : (0);
//		nodes[i].out_degree = num_succ;
//		nodes[i].ID = i;
//		if (num_succ == 0)
//			nodes[i].successors = NULL;
//		else
//		{
//			nodes[i].successors = (node**)malloc(num_succ*sizeof(node*));
//			successors_size += num_succ*sizeof(node*);
//		}
//		//cout<<"2";
//		/* fill the successor list of node[i] */
//		ulong_t succ = rand() % NODES;  // Random;
//		assert(MAX_OUTDEGREE < 500);
//		for (ulong_t j = 0; j < num_succ; j++)
//		{
//			while (succ == i)
//			{
//				succ += (rand() % (NODES / 500) + 1);
//				if (succ >= NODES)
//					succ -= NODES;
//			}
//			/* we have found a new successor which we are not already connected to */
//			nodes[i].successors[j] = &nodes[succ];
//		}
//        
//		// Kernel specific initializations
//#ifdef sgraph_teenage_follower
//		        nodes[i].teenager = (rand() % 2) ? true : false;
//#endif
//	}
//	//cout<<"3";
//#ifdef sgraph_bellman_ford
//	    cout << " Initializing weights_list for the weighted graph ..." << endl;
//	weights_size = 0;
//	weights_list = NULL;
//    
//	for (ulong_t i = 0; i < NODES; i++)
//	{
//		ulong_t d = nodes[i].out_degree;
//		if (d == 0)
//			nodes[i].weights = NULL;
//		else
//		{
//			nodes[i].weights = (ulong_t*)malloc(d*sizeof(ulong_t));
//			weights_size += d*sizeof(ulong_t);
//		}
//		for (ulong_t j = 0; j < d; j++)
//			nodes[i].weights[j] = rand() % MAX_WEIGHT + 1;
//	}
//    
//	weights_list = nodes[0].weights;
//#endif
//    
//	successors_list = nodes[0].successors;
//	reset_graph_stats();
//}
vector<string> split(const string &s, const string &seperator) {
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;
    
	while (i != s.size()) {
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}
        
		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}
;

void create_graph()
{
	// Initialize the list of lists
	// Later, we should read the graph from data sets
	nodes = (node*)malloc(NODES*sizeof(node));
#ifdef sgraph_bfs
	queue = (ulong_t*)malloc(NODES*sizeof(ulong_t));
#endif
	successors_size = 0;
	successors_list = NULL;
	//cout<<"1";
   // string file="Slashdot0811_r.txt";
     // string file="soc-LiveJournal1_r.txt";
#ifdef com_dblp_ungraph_r_txt
      string file = "com-dblp.ungraph_r.txt";
#endif
#ifdef Slashdot0811_r_txt
	string file = "Slashdot0811_r.txt";
#endif
#ifdef soc_LiveJournal1_r_txt
	string file = "soc-LiveJournal1_r.txt";
#endif
#ifdef WikiTalk_r_txt
	string file = "WikiTalk_r.txt";
#endif
#ifdef p2p_Gnutella31_r_txt
	string file = "p2p-Gnutella31_r.txt";
#endif
#ifdef com_youtube_ungraph_r_txt
	string file = "com-youtube.ungraph_r.txt";
#endif
#ifdef web_Google_r
	string file = "web-Google_r.txt";
#endif
#ifdef test
	string file = "test.txt";
#endif
#ifdef Wiki_Vote_r
	string file = "Wiki-Vote_r.txt";
#endif
#ifdef Amazon0302_r
	string file = "Amazon0302_r.txt";
#endif
#ifdef p2p_Gnutella04_r
	string file = "p2p-Gnutella04_r.txt";
#endif
#ifdef p2p_Gnutella05_r
	string file = "p2p-Gnutella05_r.txt";
#endif
#ifdef p2p_Gnutella06_r
	string file = "p2p-Gnutella06_r.txt";
#endif

#ifdef p2p_Gnutella08_r
	string file = "p2p-Gnutella08_r.txt";
#endif
#ifdef p2p_Gnutella09_r
	string file = "p2p-Gnutella09_r.txt";
#endif
#ifdef p2p_Gnutella30_r
	string file = "p2p-Gnutella30_r.txt";
#endif
	ifstream infile(file);
    //infile.open(file.data());   //将文件流对象与文件连接起来
	assert(infile.is_open());
	//  cout<<"1";
	string s;
    
	//  infile.close();             //关闭文件输入流
	for(ulong_t i = 0 ; i < NODES ; i++)
	{
		if(i%(NODES/10)==0)
			cout << "(main.cpp): Created " << i << " nodes ..." << endl;
		getline(infile, s);
        
		vector<string> v = split(s, " ");
		ulong_t num_succ = (v.size() <= 0) ? 0 : v.size() - 1;
		nodes[i].out_degree = num_succ;
		nodes[i].ID = i;
		if (num_succ == 0)
			nodes[i].successors = NULL;
		else
		{
			nodes[i].successors = (node**)malloc(num_succ*sizeof(node*));
			successors_size += num_succ*sizeof(node*);
		}
        
		for (ulong_t j = 0; j < num_succ; j++)
		{
            
			ulong_t succ = atol(v[1 + j].c_str());
			nodes[i].successors[j] = &nodes[succ];
		}
        
		// Kernel specific initializations
#ifdef sgraph_teenage_follower
#ifdef FAST_ATF
		        nodes[i].teenager = i <= 5 ? true : false;
        
#else
		nodes[i].teenager = (rand() % 2) ? true : false;
#endif
#endif
	}
    
#if(defined sgraph_bellman_ford)||(defined sgraph_bc)
	cout << " Initializing weights_list for the weighted graph ..." << endl;
	weights_size = 0;
	weights_list = NULL;
    
	for (ulong_t i = 0; i < NODES; i++)
	{
		ulong_t d = nodes[i].out_degree;
		if (d == 0)
			nodes[i].weights = NULL;
		else
		{
			nodes[i].weights = (ulong_t*)malloc(d*sizeof(ulong_t));
			weights_size += d*sizeof(ulong_t);
		}
		for (ulong_t j = 0; j < d; j++)
			nodes[i].weights[j] = rand() % MAX_WEIGHT + 1;
	}
    
	weights_list = nodes[0].weights;
#endif
    
	successors_list = nodes[0].successors;
	reset_graph_stats();
}

int main(int argc, const char * argv[]) {
	zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
	srand(0);
	cout << "(main.cpp): Kernel Name: " << "PageRank" << endl;
	cout << "(main.cpp): Create the graph with " << NODES << " nodes ..." << endl;
	create_graph();
	cout << "(main.cpp): Done Create the graph"<< endl;
	cout << "(main.cpp): Program size: " << NODES *sizeof(node) + successors_size << endl;
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
//	zsim_magic_op(ZSIM_MAGIC_OP_ROI_BEGIN);
	//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_TO);
	run_golden();
	//zsim_magic_op(ZSIM_MAGIC_OP_PIM_SWITCH_BACK);
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
	
	
	
	
//	zsim_magic_op(ZSIM_MAGIC_OP_WORK_END);
	cout << "(main.cpp): Done." << endl;
	//zsim_magic_op(ZSIM_MAGIC_OP_ROI_END);
	return 0;
}
