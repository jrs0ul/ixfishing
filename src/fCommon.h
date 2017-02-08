#ifndef FCOMMON_H
#define FCOMMON_H

#include "TextureLoader.h"

struct TFont{
    COLOR c;
    PicsContainer* pics;
    unsigned index;

    void load(PicsContainer& _pics,  unsigned _index, const char* file, const char * dir);
    void display(int x, int y, const char* text, float size = 1.0f, float spacebetween = 9.0f);
    void setcolor(float _r, float _g, float _b, float _a = 1.0f){c = COLOR(_r, _g, _b, _a);}
};
//=============================================================


 void imgframe(PicsContainer & pics, int x, int y, int x1, int y1,
                bool down, unsigned index, unsigned frame);



#endif //FCOMMON_H
