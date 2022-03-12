/**
 * @author ECE 3058 TAs
 */

#ifndef __CACHESIM_H
#define __CACHESIM_H

// Use these in code to make mem read/mem write/inst read related code more readable
#define MEMREAD 0
#define MEMWRITE 1
#define IFETCH 2

//#include "LruStack.h"
//#include "CacheBlock.h"
#include "CacheSet.h"
#include <vector>

// Please DO NOT CHANGE the following two typedefs
typedef unsigned long long addr_t;		// Data type to hold addresses
typedef unsigned long long counter_t;	// Data type to hold cache statistic variables

/**
 * Struct for a cache block. Feel free to change any of this if you want. 
 */
// typedef struct cache_block_t {
// 	int tag;
// 	int valid;
// 	int dirty;
// } cache_block_t;

/**
 * Struct for a cache set. Feel free to change any of this if you want. 
 */
// typedef struct cache_set_t {
// 	int size;			// Number of blocks in this cache set
// 	LruStack stack();			// LRU Stack 
// 	std::vector<cache_block_t> blocks; // Array of cache block structs. You will need to
// 							// 	dynamically allocate based on number of blocks
// 							//	per set. 
// } cache_set_t;

class CacheSim {
	public:
		static counter_t accesses;     // Total number of cache accesses
		static counter_t hits;         // Total number of cache hits
		static counter_t misses;       // Total number of cache misses
		static counter_t writebacks;   // Total number of writebacks

		/**
		 * Function to intialize your cache simulator with the given cache parameters. 
		 * Note that we will only input valid parameters and all the inputs will always 
		 * be a power of 2.
		 * 
		 * @param _block_size is the block size in bytes
		 * @param _cache_size is the cache size in bytes
		 * @param _ways is the associativity
		 */
		CacheSim(int block_size, int cache_size, int ways);

		/**
		 * Function to perform a SINGLE memory access to your cache. In this function, 
		 * you will need to update the required statistics (accesses, hits, misses, writebacks)
		 * and update your cache data structure with any changes necessary.
		 * 
		 * @param physical_addr is the address to use for the memory access. 
		 * @param access_type is the type of access - 0 (data read), 1 (data write) or 
		 *      2 (instruction read). We have provided macros (MEMREAD, MEMWRITE, IFETCH)
		 *      to reflect these values in cachesim.h so you can make your code more readable.
		 */
		void access(addr_t physical_add, int access_type);

		// ~CacheSim() {
			
		// }
		
	private:		
		int block_size;         // Block size
		int cache_size;         // Cache size
		int ways;               // Ways
		int num_sets;           // Number of sets
		int num_offset_bits;    // Number of offset bits
		int num_index_bits;     // Number of index bits. 

		std::vector<CacheSet*> cache;

		int simple_log_2(int x) {
			int val = 0;
			while (x > 1) {
				x /= 2;
				val++;
			}
			return val; 
		}

		addr_t create_bitmask(int x) {
			addr_t mask = 0;
			for (int i = 0; i < x; i++) {
				mask = (mask << 1) | 0b1;
			}
			return mask;
		}
};

#endif
