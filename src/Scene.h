#ifndef SCENE_H
#define SCENE_H

#include "Collisions.h"

typedef void* (*ScriptFuncPtr)(void*);

struct SceneRect{
    Rectangle_ r;
    ScriptFuncPtr func;
};



class Scene{

public:

    bool grid[10][15];
    DArray<SceneRect> HotSpots;
    unsigned BgIndex;

    int ScaleFactor;

    bool useClouds;

    void destroy(){HotSpots.destroy();}

};


#endif
