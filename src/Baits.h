#ifndef BAITS_H
#define BAITS_H
#include "DArray.h"

//----------------------------------------
struct MBait{
    unsigned long index;
    int count;
};

//----------------------------------------
struct TBait{
    char name[50];
    char units[10];
    int price;
    int countInShop;
    int usedCount;
    int maxCount;
};

//-----------------------------
class BaitData{
    DArray<TBait> baits;
public:
    unsigned long count(){return baits.count();}
    bool load(const char* path);
    TBait * getBait(unsigned long index){if (index < baits.count()) return &baits[index]; else return 0;}
    void destroy(){baits.destroy();}
    
};



#endif //BAITS_H

