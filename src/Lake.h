#ifndef LAKE_H
#define LAKE_H

#include "Vectors.h"
#include "Fish.h"
#include "Scene.h"



struct LakePoint{
    Vector3D point;
    unsigned type;
    bool locked;
    ScriptFuncPtr func;

    FishPack fes;

    char lakepics[7][255];

    LakePoint(){
        locked = false;
        func = 0;
    }

    void destroy(){fes.fish.destroy();}
};


class Lake{
public:
    DArray<LakePoint> spots;
    unsigned bgIndex;

    void destroy();

};


class LakeList{

public:
    DArray<Lake> lakes;
    void destroy();
    void load(const char* path);
};



#endif
