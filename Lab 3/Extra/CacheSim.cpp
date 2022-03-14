#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ICache.h"
#include "DCache.h"
#include "L2Cache.h"

// static counter_t accesses;     // Total number of cache accesses
// static counter_t hits;         // Total number of cache hits
// static counter_t misses;       // Total number of cache misses
// static counter_t writebacks;   // Total number of writebacks

#define CACHE_SIZE_DEF 16384
#define BLOCK_SIZE_DEF 64
#define WAYS_DEF 256

#define CACHE_SIZE_2_DEF 262144
#define BLOCK_SIZE_2_DEF 64

counter_t  DCache::accesses = 0;     // Total number of cache accesses
counter_t  DCache::hits = 0;         // Total number of cache hits
counter_t  DCache::misses = 0;       // Total number of cache misses
counter_t  DCache::writebacks = 0;   // Total number of writebacks

/**
		 * Function to print cache statistics
		 * DO NOT update what this prints.
		 */
void print_stats(L2Cache* l2_cache, ICache* i_cache, DCache* d_cache) {
    //accesses, hits, misses, wbs
    printf("\nAccesses, Hits, Misses, WBs\n");
    i_cache->print_stats();
    d_cache->print_stats();
    l2_cache->print_stats();
    printf("\nMiss Rates\n");
    i_cache->print_miss_rate();
    d_cache->print_miss_rate();
    l2_cache->print_miss_rate();
    printf("\nWriteBacks\n");
    i_cache->print_writebacks();
    d_cache->print_writebacks();
    l2_cache->print_writebacks();

}

/**
 * Function to open the trace file
 * You do not need to update this function. 
 */
FILE *open_trace(const char *filename) {
    return fopen(filename, "r");
}

int counter = 0;

/**
 * Read in next line of the trace
 * 
 * @param trace is the file handler for the trace
 * @return 0 when error or EOF and 1 otherwise. 
 */
int next_line(FILE* trace, ICache* i_cache, DCache* d_cache) {
    if (feof(trace) || ferror(trace)) return 0;
    else {
        int t;
        unsigned long long address, instr;
        fscanf(trace, "%d %llx %llx\n", &t, &address, &instr);
        if (t == IFETCH) {
            i_cache->access(address);
            //i_cache->print_last();
        } else {
            d_cache->access(address, t);
        }
        
    }
    return 1;
}

/**
 * Main function. See error message for usage. 
 * 
 * @param argc number of arguments
 * @param argv Argument values
 * @returns 0 on success. 
 */
int main(int argc, char **argv) {
    FILE *input;

    if (argc != 3) {
        fprintf(stderr, "Usage:\n  %s <trace> <block size(bytes)>"
                        " <cache size(bytes)> <ways>\n", argv[0]);
        return 1;
    }

    input = open_trace(argv[1]);
    L2Cache l2_cache(BLOCK_SIZE_2_DEF, CACHE_SIZE_2_DEF, atol(argv[2]));
    ICache i_cache(BLOCK_SIZE_DEF, CACHE_SIZE_DEF, &l2_cache);
    DCache d_cache(BLOCK_SIZE_DEF, CACHE_SIZE_DEF, &l2_cache);

    while (next_line(input, &i_cache, &d_cache));
    print_stats(&l2_cache, &i_cache, &d_cache);
    fclose(input);
    return 0;
}