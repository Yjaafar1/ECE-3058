#!/bin/bash
rm -f ece3058_cachelab_submission.tar.gz
tar -czvf ece3058_cachelab_submission.tar.gz CacheBlock.h CacheSet.h CacheSim.cpp CacheSim.h LruStack.cpp LruStack.h
echo "Done!"
echo "The files that will be submitted are:"
tar -ztvf ece3058_cachelab_submission.tar.gz