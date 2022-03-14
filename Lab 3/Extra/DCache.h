
#ifndef __DCACHE_H
#define __DCACHE_H

#include "CacheSet.h"
#include "L2Cache.h"
#include <vector>

#define DEBUG 1

// Use these in code to make mem read/mem write/inst read related code more readable
#define MEMREAD 0
#define MEMWRITE 1
#define IFETCH 2

// Please DO NOT CHANGE the following two typedefs
typedef unsigned long long addr_t;		// Data type to hold addresses
typedef unsigned long long counter_t;	// Data type to hold cache statistic variables

class DCache {
    public:
        DCache(int block_size, int cache_size, L2Cache* l2Cache);

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

        static counter_t accesses;     // Total number of cache accesses
		static counter_t hits;         // Total number of cache hits
		static counter_t misses;       // Total number of cache misses
		static counter_t writebacks;   // Total number of writebacks

		void print_stats(void) {
   	 		printf("Data: %llu, %llu, %llu, %llu\n",  accesses,  hits,  misses,  writebacks);  
		}

		void print_miss_rate(void) {
			printf("Data: %f\n", ((double)misses)/accesses);
		}

		void print_writebacks(void) {
			printf("Data: %d\n", writebacks);
		}

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

        void access_L2(addr_t physical_add, int access_type, int index_mask) {
            //missed, so access L2 cache for data
            addr_t back_invalid = this->l2Cache->access(physical_add, access_type);

            //if back-invalidate required, compute index and tag
            //and invalidate block, increment writebacks if block dirty
            if (back_invalid) {
                addr_t back_index = (back_invalid >> num_offset_bits) & index_mask;
                addr_t back_tag = (back_invalid >> (num_offset_bits + num_index_bits));
                cache[back_index]->blocks[0]->valid = 0;
                //doesn't access L2 again bc instructions state only to update memory
                if (cache[back_index]->blocks[0]->dirty)
                    DCache::writebacks++;
            }
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