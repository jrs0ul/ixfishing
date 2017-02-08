
#include "Baits.h"
#include "Xml.h"

bool BaitData::load(const char* path){
    
    Xml data;
    if (!data.load(path))
        return false;
        
    XmlNode * _baits = 0;
    _baits = data.root.getNode(L"Baits");
    if (_baits){
        for (unsigned i = 0; i < _baits->childrenCount(); i++){
            XmlNode * bait = 0;
            bait = _baits->getNode(i);
            if (bait){
                TBait b;
                char buf[100];
                
                wcstombs(buf, bait->getAttribute(0)->getValue(), 100);
                b.countInShop = atoi(buf);
                wcstombs(buf, bait->getAttribute(1)->getValue(), 100);
                b.usedCount = atoi(buf);
                wcstombs(buf, bait->getAttribute(2)->getValue(), 100);
                b.maxCount = atoi(buf);
                
                wcstombs(b.name, bait->getNode(L"Name")->getValue(), 50);
                wcstombs(buf, bait->getNode(L"Price")->getValue(), 100);
                b.price = atoi(buf);
                wcstombs(b.units, bait->getNode(L"Units")->getValue(), 10);
                
                baits.add(b);
            }
        }
    }
        
    data.destroy();
    
    return true;
}
