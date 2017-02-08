#include "Lake.h"

void Lake::destroy(){

    for (unsigned i = 0; i < spots.count(); i++)
        spots[i].destroy();
    spots.destroy();

}

