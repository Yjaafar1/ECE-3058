#include "DCache.h"

#define DEBUG 1

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
    printf("index: %s, tag %s", index, tag);
    if (DEBUG == 1) {
        printf("index: %s, tag %s", index, tag);
    }

    //if hit, increment hits and return
    if (cache[index]->blocks[0]->tag == tag && cache[index]->blocks[0]->valid) {
        cache[index]->blocks[0]->dirty = (access_type == MEMWRITE);
        DCache::hits++;
        return;
    }

    //increment misses
    DCache::misses++;

    access_L2(physical_add, access_type, index_mask);

    //if overwritten block is valid and dirty, access L2 Cache to update 
    if (cache[index]->blocks[0]->dirty && cache[index]->blocks[0]->valid) {
        addr_t override_addr = (cache[index]->blocks[0]->tag) << (num_offset_bits + num_index_bits) | (index << num_offset_bits);
        access_L2(override_addr, MEMWRITE, index_mask);
    }

    // override old block whether data is from mem or L2 Cache
    cache[index]->blocks[0]->valid = 1;
    cache[index]->blocks[0]->tag = tag;
    cache[index]->blocks[0]->dirty = (access_type == MEMWRITE); 
}