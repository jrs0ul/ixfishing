#include "fCommon.h"

//-------------------------------------------------
void TFont::display(int x, int y, const char * text, float size, float spacebetween){
 
    for (unsigned i = 0; i < strlen(text); i++){
        pics->draw(index, x+i*spacebetween, y, text[i]-32, false, size, size, 0.0f,
                    c, c);
    }
}
//-------------------------------------------------
void TFont::load(PicsContainer& _pics, unsigned _index,
                 const char* file, const char * dir){

    pics = &_pics;
    index = _index;
    char fullPath[255];
    sprintf(fullPath, "%s%s", dir, file);

    pics->loadFile(fullPath, _index, 16, 16, 1);
}
//__________________________________________}

 void textframe(PicsContainer& pics, int x, int y, int x1, int y1, 
                int tx, int ty,
                const char * s,
                TFont& font,
                bool dw){
    //frame3d(pics, x, y, x1, y1, dw);
    font.display(x + tx, y + ty, s);
}
//__________________________________________}
 void imgframe( PicsContainer& pics, int x, int y, int x1, int y1,
                bool down, unsigned index, unsigned frame){
   pics.draw(index, x, y, frame, false);
   //frame3d(pics, x, y, x1, y1, down);
}


