#ifndef MY_DATA_H
#define MY_DATA_H

#include "Baits.h"
#include "Fish.h"


class MyData{
    DArray<MBait> MyBaits;
    DArray<TCatch> Bfishes;
    double _money;
    
public:
    
    MyData(){
        _money = 0.00;
    }
    
    void addInitialBaits();
    MBait * getBait(unsigned long index);
    MBait * getBaitByType(unsigned type);
    int getBaitIndexByType(unsigned type);
    TCatch * getCatch(unsigned long index);
    bool save(const char * path);
    bool load(const char * path);
    double money(){return _money;}
    void setMoney(double m){_money = m;}
    unsigned long baitCount(){return MyBaits.count();}
    unsigned long catchCount(){return Bfishes.count();}
    void addCatch(unsigned kind, int mass);
    void addBait(unsigned kind, int count);
    int catchCountByKind(unsigned kind);
    int catchWeightByKind(unsigned kind);
    int totalCatchWeight();
    int differentFishCount();
    
    void removeBait(unsigned index){MyBaits.remove(index);}
    void removeCatchByType(unsigned type);
    
    void destroyCatch(){Bfishes.destroy();}
    void reset(){MyBaits.destroy(); Bfishes.destroy(); _money = 0.00; addInitialBaits();}
    void destroy(){MyBaits.destroy(); Bfishes.destroy();}
  
    
};


#endif
