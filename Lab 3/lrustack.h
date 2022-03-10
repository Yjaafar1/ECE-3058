#ifndef LRUSTACK_H
#define LRUSTACK_H

#include <iterator>
#include <list>
using namespace std;

class LruStack {
    public:
        /**
         * Constructor function to initialize an LRU stack for a cache set with a given <size>. This function
         * creates the LRU stack. 
         * 
         * @param size is the size of the LRU stack to initialize. 
         */
        LruStack() = default;

        /**
         * Function to get the index of the least recently used cache block, as indicated by <stack>.
         * This operation should not change/mutate your LRU stack. 
         * 
         * @return the index of the LRU cache block.
         */
        int getLru();

        /**
         * Function to mark the cache block with index <n> as MRU in <stack>. This operation should
         * change/mutate the LRU stack.
         * 
         * @param n the index to promote to MRU.  
         */
        void setMru(int n);

        // return size to determine if stack is full
        int getSize();

    private:
        int size = 0;
        list <int> priority;
};

#endif