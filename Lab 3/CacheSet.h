#include "LruStack.h"
#include "CacheBlock.h"
#include <vector>

class CacheSet {
    public:
        int getSize() {return size;}
        void setSize(int size) {this->size = size;}
        
        LruStack getStack() {return stack;}


        vector<CacheBlock> getBlocks() {return blocks;}
        void resizeBlocks(int size) {blocks.resize(size);}
    private:
        int size;
        LruStack stack;
        std::vector<CacheBlock> blocks;
};
