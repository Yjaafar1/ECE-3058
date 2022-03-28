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

        // ~CacheSet() {
        //     delete stack;
        //     int del_size = blocks.size();
        //     for (int i = 0; i < del_size; i++) {
        //         delete blocks[i];
        //     }
        // }

//         ~PersonList ()
//    {
//       size_t sz = persons.size ();
//       for (size_t i = 0; i < sz; ++i)
//           delete persons[i];
//    }

        int size;
        LruStack* stack;
        std::vector<CacheBlock*> blocks;
    private:
        
};
