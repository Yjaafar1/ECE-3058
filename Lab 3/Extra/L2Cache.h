
#ifndef __L2CACHE_H
#define __L2CACHE_H

#include "CacheSet.h"
#include <vector>

// Use these in code to make mem read/mem write/inst read related code more readable
#define MEMREAD 0
#define MEMWRITE 1
#define IFETCH 2

// Please DO NOT CHANGE the following two typedefs
typedef unsigned long long addr_t;		// Data type to hold addresses
typedef unsigned long long counter_t;	// Data type to hold cache statistic variables

class L2Cache {
    public:
        L2Cache(int block_size, int cache_size, int ways);

        //if back invalidation needed, gives address (tag and set) of removed block
        addr_t access(addr_t physical_add, int access_type);

        static counter_t accesses;     // Total number of cache accesses
		static counter_t hits;         // Total number of cache hits
		static counter_t misses;       // Total number of cache misses
		static counter_t writebacks;   // Total number of writebacks

    private:
        
        int simple_log_2(int x) {
			int val = 0;
			while (x > 1) {
				x /= 2;
				val++;
			}
			return val; 
		}

        int block_size;         // Block size
		int cache_size;         // Cache size
		int ways;               // Ways
		int num_sets;           // Number of sets
		int num_offset_bits;    // Number of offset bits
		int num_index_bits;     // Number of index bits. 
        std::vector<CacheSet*> cache;
};

#endif