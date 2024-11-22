#include "Settlement.h"

Settlement::Settlement(const string &newName,SettlementType newType)
:name(newName)
,type(newType)
{};

const string &Settlement::getName() const{return name;};