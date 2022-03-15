// /**
//  * @author ECE 3058 TAs
//  */
#include "L2Cache.h"

#define DEBUG 1

counter_t L2Cache::accesses = 0;     // Total number of cache accesses
counter_t L2Cache::hits = 0;         // Total number of cache hits
counter_t L2Cache::misses = 0;       // Total number of cache misses
counter_t L2Cache::writebacks = 0;   // Total number of writebacks
counter_t L2Cache::traffic = 0;


L2Cache::L2Cache(int block_size, int cache_size, int ways) {
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

// access type determines dirty bit?
addr_t L2Cache::access(addr_t physical_add, int access_type) {

    L2Cache::accesses++;
    addr_t index_mask = create_bitmask(num_index_bits);
    addr_t index = (physical_add >> num_offset_bits) & index_mask;
    addr_t tag = (physical_add >> (num_offset_bits + num_index_bits));

    if (DEBUG == 1) {
        printf("L2: index: %llo, tag %llo\n", index, tag);
    }
    
    // hit or miss 
    // full or not full
    // wb or no wb
    for (int i = 0; i < cache[index]->size; i++) {
        if (cache[index]->blocks[i]->tag == tag && cache[index]->blocks[i]->valid) {
            L2Cache::hits++;
            cache[index]->stack->setMru(i);
            if (access_type == MEMWRITE) {
                cache[index]->blocks[i]->dirty = 1;
            } else {
                //traffic = L1 miss, but L2 hit
                //sim does not set dirty bit as high when just bringing data from L2 to L1
                printf("-- L2 hit, L1 miss\n");
                L2Cache::traffic++;
            }
            return 0;
        }
    }
    
    //do something for miss
    // also hit?
    L2Cache::misses++;
    // case if empty blocks available after miss
    if (cache[index]->stack->getSize() < cache[index]->size) {
        int emptyIndex = cache[index]->stack->getSize();
        if (access_type == MEMWRITE) {
            cache[index]->blocks[emptyIndex]->dirty = 1;
        }
        cache[index]->blocks[emptyIndex]->valid = 1;
        cache[index]->blocks[emptyIndex]->tag = tag;
        cache[index]->stack->setMru(emptyIndex);
    // if no empty blocks available, replace LRU
    // more dirty bit stuff
    } else {
        int lruIndex = cache[index]->stack->getLru();
        if (cache[index]->blocks[lruIndex]->dirty) {
            L2Cache::writebacks++;
        }
        if (access_type == MEMWRITE) {
            cache[index]->blocks[lruIndex]->dirty = 1;
        } else {
            cache[index]->blocks[lruIndex]->dirty = 0;
        }
        cache[index]->blocks[lruIndex]->tag = tag;
        cache[index]->stack->setMru(lruIndex);
        //traffic = sending back invalidation
        printf("-- Back Invalidation\n");
        L2Cache::traffic++;
        return (cache[index]->blocks[0]->tag) << (num_offset_bits + num_index_bits) | (index << num_offset_bits);
    }
    return 0;
}
