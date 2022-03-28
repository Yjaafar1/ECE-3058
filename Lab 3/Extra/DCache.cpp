#include "DCache.h"

#define DEBUG 1

counter_t  DCache::accesses = 0;     // Total number of cache accesses
counter_t  DCache::hits = 0;         // Total number of cache hits
counter_t  DCache::misses = 0;       // Total number of cache misses
counter_t  DCache::writebacks = 0;   // Total number of writebacks
counter_t  DCache::traffic = 0;

DCache::DCache(int block_size, int cache_size, L2Cache* l2Cache) {
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
void DCache::access(addr_t physical_add, int access_type) {

    DCache::accesses++;
    addr_t index_mask = create_bitmask(num_index_bits);
    addr_t index = (physical_add >> num_offset_bits) & index_mask;
    addr_t tag = (physical_add >> (num_offset_bits + num_index_bits));

    if (DEBUG == 1) {
        printf("Data: index: %llo, tag %llo\n", index, tag);
    }

    //if hit, increment hits and return
    if (cache[index]->blocks[0]->tag == tag && cache[index]->blocks[0]->valid) {
        cache[index]->blocks[0]->dirty = (access_type == MEMWRITE);
        DCache::hits++;
        return;
    }

    //increment misses
    DCache::misses++;

    //if overwritten block is valid and dirty, access L2 Cache to update 
    if (cache[index]->blocks[0]->dirty && cache[index]->blocks[0]->valid) {
        addr_t override_addr = (cache[index]->blocks[0]->tag) << (num_offset_bits + num_index_bits) | (index << num_offset_bits);
        //traffic = dirty line evicted from L1 and sent to L2
        printf("-- Evict dirty line\n");
        DCache::traffic++;
        access_L2(override_addr, MEMWRITE, index_mask);
    }

    //can this be a mem_write?
    //what if you need to pull from mem into both L1 and L2? Do you have 
    //the L2 block as dirty?
    //I think this should always just not set it dirty
    access_L2(physical_add, 0, index_mask);

    // override old block whether data is from mem or L2 Cache
    cache[index]->blocks[0]->valid = 1;
    cache[index]->blocks[0]->tag = tag;
    cache[index]->blocks[0]->dirty = (access_type == MEMWRITE); 
}