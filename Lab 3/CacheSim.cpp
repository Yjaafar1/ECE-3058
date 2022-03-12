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

    //this->cache(num_sets);
    
    for (int i = 0; i < num_sets; i++) {
        //printf("%d \n", cache[i]);
        cache.push_back(new CacheSet(ways));
        printf("%d \n", cache[i]);
    } 
    cache[0]->stack;
    //printf("%d \n", cache[0]->stack->getLru());
}
int a;
int d;
// access type determines dirty bit?
void CacheSim::access(addr_t physical_add, int access_type) {
    //printf("%d \n", cache[d].blocks[a].valid);

    CacheSim::accesses++;
    addr_t index_mask = create_bitmask(num_index_bits);
    addr_t index = (physical_add >> num_offset_bits) & index_mask;
    addr_t tag = (physical_add >> (num_offset_bits + num_index_bits));
    //printf("%d \n", tag);
    
    // hit or miss 
    // full or not full
    // wb or no wb
    printf("Here \n");
    printf("%d \n", cache[index]->size);
    printf("Here \n");
    for (int i = 0; i < cache[index]->size; i++) {
        printf("Here \n");
        //printf("%d \n", cache[index].blocks[i].valid);
        if (cache[index]->blocks[i]->tag == tag && cache[index]->blocks[i]->valid) {
            //printf("Here \n");
            CacheSim::hits++;
            cache[index]->stack->setMru(i);

            if (access_type == MEMWRITE) {
                cache[index]->blocks[i]->dirty = 1;
            }

            return;
        }
    }
    
    //do something for miss
    // also hit?
    CacheSim::misses++;
    // case if empty blocks available after miss
    if (cache[index]->stack->getSize() < cache[index]->size) {
        //printf("%d \n", cache[index].stack.getSize());
        int emptyIndex = cache[index]->stack->getSize();
        a = emptyIndex;
        //printf("%d \n", emptyIndex);
        if (access_type == MEMWRITE) {
            cache[index]->blocks[emptyIndex]->dirty = 1;
        }
        cache[index]->blocks[emptyIndex]->valid = 1;
        cache[index]->blocks[emptyIndex]->tag = tag;
        cache[index]->stack->setMru(emptyIndex);
        //printf("Here \n");
    // if no empty blocks available, replace LRU
    // more dirty bit stuff
    } else {
        int lruIndex = cache[index]->stack->getLru();
        if (cache[index]->blocks[lruIndex]->dirty) {
            CacheSim::writebacks++;
        }
        cache[index]->blocks[lruIndex]->tag = tag;
        cache[index]->stack->setMru(lruIndex);
    }

    //printf("%d \n", cache[d].blocks[a].valid);
    
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
    next_line(input, cache);
    next_line(input, cache);
    next_line(input, cache);
    //while (next_line(input, cache));
    print_stats();
    fclose(input);
    return 0;
}

