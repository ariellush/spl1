#include "Settlement.h"

//Simple constructor
Settlement::Settlement(const string &newName,SettlementType newType)
:name(newName)
,type(newType)
{};


//name getter
const string &Settlement::getName() const{return name;};

//type getter
SettlementType Settlement::getType() const{return type;};


const string Settlement::toString() const
{
    string typeStr;
    switch (type)
    {
        case SettlementType::VILLAGE: typeStr = "Village";
        break;
        case SettlementType::CITY: typeStr ="City";
        break;
        case SettlementType::METROPOLIS: typeStr = "Metropolis";
        break;
        default: typeStr = "undefined";
        break;
    }

    return "Settlement - "+name+" | type: "+typeStr;
};
Settlement& Settlement :: operator=(const Settlement &other){};