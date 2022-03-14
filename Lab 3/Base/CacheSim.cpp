// /**
//  * @author ECE 3058 TAs
//  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CacheSim.h"

counter_t CacheSim::accesses = 0;     // Total number of cache accesses
counter_t CacheSim::hits = 0;         // Total number of cache hits
counter_t CacheSim::misses = 0;       // Total number of cache misses
counter_t CacheSim::writebacks = 0;   // Total number of writebacks


CacheSim::CacheSim(int block_size, int cache_size, int ways) {
    this->block_size = block_size;
    this->cache_size = cache_size;
    this->ways = ways;

    this->num_sets = cache_size / (ways * block_size);
    this->num_index_bits = simple_log_2(num_sets);
    this->num_offset_bits = simple_log_2(block_size);

    for (int i = 0; i < num_sets; i++) {
        cache.push_back(new CacheSet(ways));
    } 
}

void CacheSim::access(addr_t physical_add, int access_type) {

    CacheSim::accesses++;
    addr_t index_mask = create_bitmask(num_index_bits);
    addr_t index = (physical_add >> num_offset_bits) & index_mask;
    addr_t tag = (physical_add >> (num_offset_bits + num_index_bits));
    
    // check at target index for valid block and matching tag
    // if hit, set flags as needed and set MRU
    for (int i = 0; i < cache[index]->size; i++) {
        if (cache[index]->blocks[i]->tag == tag && cache[index]->blocks[i]->valid) {
            CacheSim::hits++;
            cache[index]->stack->setMru(i);

            if (access_type == MEMWRITE) {
                cache[index]->blocks[i]->dirty = 1;
            }

            return;
        }
    }
    
    // if miss, check if empty block available at index
    // write to this empty box if available 
    // set relevant flags and MRU
    CacheSim::misses++;
    if (cache[index]->stack->getSize() < cache[index]->size) {
        int emptyIndex = cache[index]->stack->getSize();
        if (access_type == MEMWRITE) {
            cache[index]->blocks[emptyIndex]->dirty = 1;
        }
        cache[index]->blocks[emptyIndex]->valid = 1;
        cache[index]->blocks[emptyIndex]->tag = tag;
        cache[index]->stack->setMru(emptyIndex);
    // if no empty blocks available, replace LRU
    // increment writebacks if block dirty
    // set MRU and new block flags
    } else {
        int lruIndex = cache[index]->stack->getLru();
        if (cache[index]->blocks[lruIndex]->dirty) {
            CacheSim::writebacks++;
        }
        if (access_type == MEMWRITE) {
            cache[index]->blocks[lruIndex]->dirty = 1;
        } else {
            cache[index]->blocks[lruIndex]->dirty = 0;
        }
        cache[index]->blocks[lruIndex]->tag = tag;
        cache[index]->stack->setMru(lruIndex);
    }
    
}

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
    print_stats();
    fclose(input);
    return 0;
}

