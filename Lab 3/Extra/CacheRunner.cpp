#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ICache.h"
#include "DCache.h"
#include "L2Cache.h"


#define CACHE_SIZE_DEF 16384
#define BLOCK_SIZE_DEF 64
#define WAYS_DEF 256

#define CACHE_SIZE_2_DEF 262144
#define BLOCK_SIZE_2_DEF 64

/**
		 * Function to print cache statistics
		 * DO NOT update what this prints.
		 */
void print_stats(void) {
    printf("%llu, %llu, %llu, %llu\n", CacheSim::accesses, CacheSim::hits, CacheSim::misses, CacheSim::writebacks);  
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
int next_line(FILE* trace, CacheSim cache) {
    if (feof(trace) || ferror(trace)) return 0;
    else {
        int t;
        unsigned long long address, instr;
        fscanf(trace, "%d %llx %llx\n", &t, &address, &instr);
        //printf("%d %llx %llx \n", t, address, instr);
        cache.access(address, t);
        //printf("%d \n", counter++);
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

    if (argc != 5) {
        fprintf(stderr, "Usage:\n  %s <trace> <block size(bytes)>"
                        " <cache size(bytes)> <ways>\n", argv[0]);
        return 1;
    }

    input = open_trace(argv[1]);
    CacheSim cache(atol(argv[2]), atol(argv[3]), atol(argv[4]));
    // next_line(input, cache);
    // next_line(input, cache);
    // next_line(input, cache);
    while (next_line(input, cache));
    print_stats();
    fclose(input);
    return 0;
}