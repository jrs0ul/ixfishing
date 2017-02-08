
#include "MyData.h"

void MyData::addInitialBaits(){
    MBait m;
    m.index = 1;
    m.count = 100;
    MyBaits.add(m);
    _money = 0.00;
}
//-----------------------
bool MyData::load(const char * path){
    FILE * f = 0;
    f = fopen(path, "rb");
    if (!f)
        return false;
    size_t res;
    res = fread(&_money, 1, sizeof(double), f);
    int baitsC = 0;// =(int)MyBaits.count(); 
    res = fread(&baitsC, 1, sizeof(int), f);
    for (int i = 0; i < baitsC; i++){
        MBait b;
        res = fread(&(b.index), 1, sizeof(int), f);
        res = fread(&(b.count), 1, sizeof(int), f);
        MyBaits.add(b);
    }
    int fishC = 0;// =(int)Bfishes.count(); 
    res = fread(&fishC, 1, sizeof(int), f);
    for (int i = 0; i < fishC; i++){
        TCatch c; //= &Bfishes[i];
        res = fread(&(c.kind), 1, sizeof(int), f);
        res = fread(&(c.weight), 1, sizeof(int), f);
        Bfishes.add(c);
    }
    
    fclose(f);
    
    return true;
}
//-----------------------
bool MyData::save(const char * path){
    FILE * f = 0;
    f = fopen(path, "wb+");
    if (!f)
        return false;
    fwrite(&_money, 1, sizeof(double), f);
    int baitsC =(int)MyBaits.count(); 
    fwrite(&baitsC, 1, sizeof(int), f);
    for (unsigned long i = 0; i < MyBaits.count();i++){
        MBait* b = &MyBaits[i];
        fwrite(&(b->index), 1, sizeof(int), f);
        fwrite(&(b->count), 1, sizeof(int), f);
    }
    int fishC =(int)Bfishes.count(); 
    fwrite(&fishC, 1, sizeof(int), f);
    for (unsigned long i = 0; i < Bfishes.count();i++){
        TCatch* c = &Bfishes[i];
        fwrite(&(c->kind), 1, sizeof(int), f);
        fwrite(&(c->weight), 1, sizeof(int), f);
    }
        
    fclose(f);
    
    return true;
}

//------------------------
int MyData::catchCountByKind(unsigned kind){
    int result = 0;
    for (unsigned i = 0; i < Bfishes.count(); i++){
        if (Bfishes[i].kind == kind){
            result++;
        }
    }
    return result;
}
//------------------------
int MyData::differentFishCount(){
    int result = 0;
    DArray<int> kinds;
    for (unsigned i = 0; i < Bfishes.count(); i++){
        int collision = 0;
        for (unsigned a = 0; a < kinds.count(); a++){
            
            if (kinds[a] == (int)Bfishes[i].kind)
                collision++;
        }
        if (!collision)
            kinds.add(Bfishes[i].kind); 
    }
    result = (int)kinds.count();
    kinds.destroy();
    
    return result;
}


//------------------------
int MyData::catchWeightByKind(unsigned kind){
    int result = 0;
    for (unsigned i = 0; i < Bfishes.count(); i++){
        if (Bfishes[i].kind == kind){
            result += Bfishes[i].weight;
        }
    }
    return result;
}
//-----------------------
int MyData::totalCatchWeight(){
    int result = 0;
    for (unsigned i = 0; i < Bfishes.count(); i++)        
        result += Bfishes[i].weight;

    return result;
}

//-------------------------
MBait * MyData::getBait(unsigned long index){
    if (index < MyBaits.count())
        return &MyBaits[index];
    return 0;
}
//--------------------------
MBait * MyData::getBaitByType(unsigned type){
    
    for (unsigned i = 0; i < MyBaits.count(); i++){
        if (MyBaits[i].index == type)
            return &MyBaits[i];
    }
    return 0;
}
//--------------------------
int MyData::getBaitIndexByType(unsigned type){
    for (unsigned i = 0; i < MyBaits.count(); i++){
        if (MyBaits[i].index == type)
            return (int)i;
    }
    return -1;
}

//--------------------------
TCatch * MyData::getCatch(unsigned long index){
    if (index < Bfishes.count())
        return &Bfishes[index];
    return 0;
}
//-------------------------
void MyData::addCatch(unsigned kind, int mass){
    TCatch c;
    c.kind = kind;
    c.weight = mass;
    Bfishes.add(c);
    
}
//-------------------------
void MyData::addBait(unsigned kind, int count){
    MBait c;
    c.index = kind;
    c.count = count;
    MyBaits.add(c);
    
}
//------------------------
void MyData::removeCatchByType(unsigned kind){

    for (int i = (int)(Bfishes.count() - 1); i >= 0; i--){
        printf("%d\n", i);
        if (Bfishes[i].kind == kind){
            Bfishes.remove(i);
        }
    }

}
