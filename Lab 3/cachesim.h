/**
 * @author ECE 3058 TAs
 */

#ifndef __CACHESIM_H
#define __CACHESIM_H

// Use these in code to make mem read/mem write/inst read related code more readable
#define MEMREAD 0
#define MEMWRITE 1
#define IFETCH 2

#include "LruStack.h"
#include <vector>

// Please DO NOT CHANGE the following two typedefs
typedef unsigned long long addr_t;		// Data type to hold addresses
typedef unsigned long long counter_t;	// Data type to hold cache statistic variables

/**
 * Struct for a cache block. Feel free to change any of this if you want. 
 */
typedef struct cache_block_t {
	int tag = 0;
	int valid = 0;
	int dirty = 0;
} cache_block_t;

/**
 * Struct for a cache set. Feel free to change any of this if you want. 
 */
typedef struct cache_set_t {
	int size = 0;				// Number of blocks in this cache set
	LruStack stack;			// LRU Stack 
	std::vector<cache_block_t> blocks;
	// cache_block_t* blocks;	// Array of cache block structs. You will need to
	// 						// 	dynamically allocate based on number of blocks
	// 						//	per set. 
} cache_set_t;

class CacheSim {
	public:
		CacheSim(int block_size, int cache_size, int ways);

		// //be extra careful with destructor
		// ~CacheSim() {
		// 	delete[] cache;
		// }

		
	private:
		counter_t accesses = 0;     // Total number of cache accesses
		counter_t hits = 0;         // Total number of cache hits
		counter_t misses = 0;       // Total number of cache misses
		counter_t writebacks = 0;   // Total number of writebacks

		
		int block_size;         // Block size
		int cache_size;         // Cache size
		int ways;               // Ways
		int num_sets;           // Number of sets
		int num_offset_bits;    // Number of offset bits
		int num_index_bits;     // Number of index bits. 

		std::vector<cache_set_t> cache;
		// cache_set_t* cache;     // Data structure for the cache

		int simple_log_2(int x) {
			int val = 0;
			while (x > 1) {
				x /= 2;
				val++;
			}
			return val; 
		}
};




void init(int block_size, int cache_size, int ways);
void access(addr_t physical_add, int access_type);
//void cachesim_cleanup(void);
void print_stats(void);

#endif
