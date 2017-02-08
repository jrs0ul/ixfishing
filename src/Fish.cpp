#include "Fish.h"
#include "Xml.h"


void       FishData::destroy(){
    for (unsigned long i = 0; i < data.count(); i++)
        data[i].destroy();
    data.destroy();
    
}
//-------------------
bool       FishData::load(const char * path){
    
    Xml d;
    if (!d.load(path))
        return false;
    
    XmlNode * fishes = 0;
    fishes = d.root.getNode(L"Fishes");
    if (fishes){
        for (unsigned long i = 0; i < fishes->childrenCount(); i++){
            XmlNode * fish = 0;
            fish = fishes->getNode(i);
            if (fish){
                FishType ft;
                char buf[100];
                wcstombs(buf, fish->getAttribute(0)->getValue(), 100);
                ft.picture = atoi(buf);
                wcstombs(buf, fish->getAttribute(1)->getValue(), 100);
                ft.response = atoi(buf);
                wcstombs(buf, fish->getAttribute(2)->getValue(), 100);
                ft.price = atoi(buf);
                wcstombs(ft.name, fish->getNode(L"Name")->getValue(), 255);
                
                XmlNode * depth = 0;
                depth = fish->getNode(L"Depth");
                if (depth){
                    wcstombs(buf, depth->getAttribute(0)->getValue(), 100);
                    ft.maxdepth = atoi(buf);
                    wcstombs(buf, depth->getAttribute(1)->getValue(), 100); 
                    ft.mindepth = atoi(buf);
                }
                
                XmlNode * weight = 0;
                weight = fish->getNode(L"Weight");
                if (weight){
                    wcstombs(buf, weight->getAttribute(0)->getValue(), 100);
                    ft.maxweight = atoi(buf);
                    wcstombs(buf, weight->getAttribute(1)->getValue(), 100); 
                    ft.minweight = atoi(buf);
                }
                
                
                XmlNode * foods = 0;
                foods = fish->getNode(L"Eats");
                if (foods){
                    for (unsigned long a = 0; a < foods->childrenCount(); a++) {
                        XmlNode * food = foods->getNode(a);
                        TFood tf;
                        wcstombs(buf, food->getAttribute(0)->getValue(), 100);
                        tf.maistas = atoi(buf);
                        wcstombs(buf, food->getAttribute(1)->getValue(), 100); 
                        tf.kvapas = atoi(buf);
                        ft.foods.add(tf);
                        
                    }
                }
                
                data.add(ft);
            }
        }
    }
    
    d.destroy();
    
    return true;
    
}
//-------------------
FishType * FishData::getType(unsigned long index){
    if (index < data.count())
        return &data[index];
    return 0;
}
//-----------------
bool FishType::isEatingThis(unsigned index){
    for (unsigned i = 0; i < foods.count(); i++){
        if (index == foods[i].maistas)
            return true;
    }
    return false;
}
//---------------
unsigned FishType::getFoodIndex(unsigned foodType){
    for (unsigned i = 0; i < foods.count(); i++){
        if (foodType == foods[i].maistas)
            return i;
    }
    return 0;
}

