#ifndef __CACHEBLOCK_H
#define __CACHEBLOCK_H

class CacheBlock {
    public:
        int getTag() {return tag;}
        int getValid() {return valid;}
        int getDirty() {return dirty;}

        void setDirty(int dirty) {this->dirty = dirty;}
        void setValid(int valid) {this->valid = valid;} 
        void setTag(int tag) {this->tag = tag;}

        int tag;
        int valid;
        int dirty;

    private:
        
};


#endif