
#ifndef __ICACHE_H
#define __ICACHE_H



#include "CacheSet.h"
#include <vector>

class ICache {
    public:
        Cache(int block_size, int cache_size, int ways);

        void access();


    private:
        std::vector<CacheSet*> cache;
        L2Cache* l2Cache;
};

#endif