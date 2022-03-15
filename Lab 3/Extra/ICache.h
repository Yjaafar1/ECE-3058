
#ifndef __ICACHE_H
#define __ICACHE_H

#include "CacheSet.h"
#include "L2Cache.h"
#include <vector>

// Use these in code to make mem read/mem write/inst read related code more readable
#define MEMREAD 0
#define MEMWRITE 1
#define IFETCH 2

// Please DO NOT CHANGE the following two typedefs
typedef unsigned long long addr_t;		// Data type to hold addresses
typedef unsigned long long counter_t;	// Data type to hold cache statistic variables

class ICache {
    public:
        ICache(int block_size, int cache_size, L2Cache* l2Cache);

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
		void access(addr_t physical_add);

        static counter_t accesses;     // Total number of cache accesses
		static counter_t hits;         // Total number of cache hits
		static counter_t misses;       // Total number of cache misses
		static counter_t writebacks;   // Total number of writebacks
		static counter_t traffic;   // Total number of writebacks

		void print_stats(void) {
   	 		printf("Instr: %llu, %llu, %llu, %llu\n",  accesses,  hits,  misses,  writebacks);  
		}

		void print_miss_rate(void) {
			printf("Instr: %f\n", ((double)misses)/accesses);
		}

		void print_miss_rate_global(counter_t global) {
			printf("Instr: %f\n", ((double)misses)/global);
		}

		void print_writebacks(void) {
			printf("Instr: %d\n", writebacks);
		}

		// void print_last(void) {
   	 	// 	printf("%d, %d\n",  last_index,  last_tag);  
		// }

    private:
        
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

        //int writebacks;

        int block_size;         // Block size
		int cache_size;         // Cache size
		int num_sets;           // Number of sets
		int num_offset_bits;    // Number of offset bits
		int num_index_bits;     // Number of index bits. 
        std::vector<CacheSet*> cache;
        L2Cache* l2Cache;
};

#endif