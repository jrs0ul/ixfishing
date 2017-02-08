#ifndef FISH_H
#define FISH_H

#include "DArray.h"

struct TFood{
    unsigned maistas;
    int kvapas;
};
//--------------------------
struct FishType{

    int maxdepth, mindepth;
    int maxweight, minweight;
    DArray<TFood> foods;
    char name[255];
    int response;//lag
    unsigned picture;
    int price;
    
    void destroy(){foods.destroy();}
    bool isEatingThis(unsigned index);
    unsigned getFoodIndex(unsigned foodType);
};
//--------------------------
struct Fish{
    int x, y;
    int depth;
    int dir;
    int maxdepth, mindepth;
    unsigned kind;
    int mase;
    int tim;
    bool dead;

    Fish(){
        dead = false;
    }
};

//----------------------
class FishData{
    DArray<FishType> data;
    
public:
    void destroy();
    bool load(const char * path);
    FishType * getType(unsigned long index);
    unsigned long count(){return data.count();}
};

//----------------------
struct TCatch{
    unsigned kind;
    int weight;
};
//---------------------
struct FishPackElement{
    unsigned kind;
    int count;
};
//----------------
struct FishPack{
    DArray<FishPackElement> fish;
};



#endif //FISH_H
