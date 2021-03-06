#include "ICache.h"
#define DEBUG 1
counter_t  ICache::accesses = 0;     // Total number of cache accesses
counter_t  ICache::hits = 0;         // Total number of cache hits
counter_t  ICache::misses = 0;       // Total number of cache misses
counter_t  ICache::writebacks = 0;   // Total number of writebacks
counter_t  ICache::traffic = 0;   // Total number of writebacks

ICache::ICache(int block_size, int cache_size, L2Cache* l2Cache) {
    this->block_size = block_size;
    this->cache_size = cache_size;
    this->l2Cache = l2Cache;

    this->num_sets = cache_size / block_size;
    this->num_index_bits = simple_log_2(num_sets);
    this->num_offset_bits = simple_log_2(block_size);

    for (int i = 0; i < num_sets; i++) {
        //cache set maintains LRU, I just don't have to use it
        cache.push_back(new CacheSet(1));
    } 
    //double check all correct relative to direct map
}

// access type determines dirty bit?
void ICache::access(addr_t physical_add) {

    ICache::accesses++;
    addr_t index_mask = create_bitmask(num_index_bits);
    addr_t index = (physical_add >> num_offset_bits) & index_mask;
    addr_t tag = (physical_add >> (num_offset_bits + num_index_bits));

    if (DEBUG == 1) {
        printf("Instr: index: %llo, tag %llo\n", index, tag);
    }

    //No dirty bits with Instruction Cache

    //if hit, increment hits and return
    if (cache[index]->blocks[0]->tag == tag && cache[index]->blocks[0]->valid) {
        ICache::hits++;
        return;
    }

    //increment misses
    ICache::misses++;

    //missed, so access L2 cache for data (instruction)
    addr_t back_invalid = this->l2Cache->access(physical_add, IFETCH);

    //if back-invalidate required, compute index and tag
    //and invalidate block, increment writebacks if block dirty
    if (back_invalid) {
        addr_t back_index = (back_invalid >> num_offset_bits) & index_mask;
        addr_t back_tag = (back_invalid >> (num_offset_bits + num_index_bits));
        cache[back_index]->blocks[0]->valid = 0;
    }

    // assume dirty bit never encountered for instruction cache
    // override old block whether data is from mem or L2 Cache
    cache[index]->blocks[0]->valid = 1;
    cache[index]->blocks[0]->tag = tag;

    // //Two scenarios for placing data in cache
    // //1) block empty or invalidated, free to replace
    // if (cache[index]->blocks[0]->valid && cache[index]->blocks[0]->dirty) {
        
    // } else {
    //     // if (cache[index]->blocks[0]->dirty) {
    //     //     l2Cache->access(physical_add, MEMWRITE);
    //     // }
    // }
    

    

    // //accessing L2 returns true if a back invalidation is necessary
    
    
    // //add removal from LRU for back invalidation

    // // case if empty blocks available after miss
    // if (cache[index]->stack->getSize() < cache[index]->size) {
    //     int emptyIndex = cache[index]->stack->getSize();
    //     cache[index]->blocks[emptyIndex]->valid = 1;
    //     cache[index]->blocks[emptyIndex]->tag = tag;
    //     cache[index]->stack->setMru(emptyIndex);
    // // if no empty blocks available, replace LRU
    // // more dirty bit stuff
    // } else {
    //     int lruIndex = cache[index]->stack->getLru();
    //     if (cache[index]->blocks[lruIndex]->dirty) {
    //         CacheSim::writebacks++;
    //     }
    //     if (access_type == MEMWRITE) {
    //         cache[index]->blocks[lruIndex]->dirty = 1;
    //     } else {
    //         cache[index]->blocks[lruIndex]->dirty = 0;
    //     }
    //     cache[index]->blocks[lruIndex]->tag = tag;
    //     cache[index]->stack->setMru(lruIndex);
    // }
    
}