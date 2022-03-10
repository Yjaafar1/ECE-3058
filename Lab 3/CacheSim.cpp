// /**
//  * @author ECE 3058 TAs
//  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CacheSim.h"

CacheSim::CacheSim(int block_size, int cache_size, int ways) {
    this->block_size = block_size;
    this->cache_size = cache_size;
    this->ways = ways;

    this->num_sets = cache_size / (ways * block_size);
    this->num_index_bits = simple_log_2(num_sets);
    this->num_offset_bits = simple_log_2(block_size);

    this->cache.resize(num_sets);
    
    for (int i = 0; i < num_sets; i++) {
        cache_set_t set = cache[i];
        set.size = ways;
        set.blocks.resize(set.size);
    } 
    
}

void CacheSim::access(addr_t physical_add, int access_type) {

}

// /**
//  * Function to perform a SINGLE memory access to your cache. In this function, 
//  * you will need to update the required statistics (accesses, hits, misses, writebacks)
//  * and update your cache data structure with any changes necessary.
//  * 
//  * @param physical_addr is the address to use for the memory access. 
//  * @param access_type is the type of access - 0 (data read), 1 (data write) or 
//  *      2 (instruction read). We have provided macros (MEMREAD, MEMWRITE, IFETCH)
//  *      to reflect these values in cachesim.h so you can make your code more readable.
//  */
// void cachesim_access(addr_t physical_addr, int access_type) {
//     ////////////////////////////////////////////////////////////////////
//     //  TODO: Write the code needed to perform a cache access on your
//     //  cache simulator. Some things to remember:
//     //      - When it is a cache hit, you SHOULD NOT bring another cache 
//     //        block in.
//     //      - When it is a cache miss, you should bring a new cache block
//     //        in. If the set is full, evict the LRU block.
//     //      - Remember to update all the necessary statistics as necessary
//     //      - Remember to correctly update your valid and dirty bits.  
//     ////////////////////////////////////////////////////////////////////
    
//     ////////////////////////////////////////////////////////////////////
//     //  End of your code   
//     ////////////////////////////////////////////////////////////////////
// }

// /**
//  * Function to free up any dynamically allocated memory you allocated
//  */
// void cachesim_cleanup() {
//     ////////////////////////////////////////////////////////////////////
//     //  TODO: Write the code to do any heap allocation cleanup
//     ////////////////////////////////////////////////////////////////////

//     ////////////////////////////////////////////////////////////////////
//     //  End of your code   
//     ////////////////////////////////////////////////////////////////////
// }

// /**
//  * Function to print cache statistics
//  * DO NOT update what this prints.
//  */
// void cachesim_print_stats() {
//     printf("%llu, %llu, %llu, %llu\n", accesses, hits, misses, writebacks);  
// }

/**
 * Function to open the trace file
 * You do not need to update this function. 
 */
FILE *open_trace(const char *filename) {
    return fopen(filename, "r");
}

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
        cache.access(address, t);
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
    while (next_line(input, cache));
    cache.print_stats();
    fclose(input);
    return 0;
}

