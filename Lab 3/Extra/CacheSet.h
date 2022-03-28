#ifndef __CACHESET_H
#define __CACHESET_H

#include "LruStack.h"
#include "CacheBlock.h"
#include <vector>

class CacheSet {
    public:
        CacheSet(int size) {
            this->size = size;
            this->stack = new LruStack();
            for (int i = 0; i < size; i++) {
                blocks.push_back(new CacheBlock());
            }
        }

        int getSize() {return size;}
        void setSize(int size) {this->size = size;}
        
        LruStack* getStack() {return stack;}


        vector<CacheBlock*> getBlocks() {return blocks;}
        void resizeBlocks(int size) {blocks.resize(size);}

        int size;
        LruStack* stack;
        std::vector<CacheBlock*> blocks;
    private:
        
};

#endif
