#pragma once
#include "Facility.h"

FacilityType :: FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
    name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score) {}

const string &FacilityType :: getName() const{
    return name;
}

int FacilityType :: getLifeQualityScore() const {
    return lifeQuality_score;
}

int FacilityType :: getEnvironmentScore() const {
    return environment_score;
}

int FacilityType :: getEconomyScore() const {
    return economy_score;
}

FacilityCategory FacilityType :: getCategory() const {
    return category;
}

Facility :: Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : FacilityType :: FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score), settlementName(settlementName), timeLeft(price){
        status = FacilityStatus :: UNDER_CONSTRUCTIONS;
    }

Facility :: Facility(FacilityType &type, const string &settlementName)
    : FacilityType :: FacilityType(type), settlementName(settlementName), timeLeft(price){
        status = FacilityStatus :: UNDER_CONSTRUCTIONS;
    }

const string &Facility:: getSettlementName() const {
    return settlementName;
}    

const int Facility :: getTimeLeft() const {
    return timeLeft;
}


void Facility :: setStatus(FacilityStatus status) {
    this->status = status;
}

const FacilityStatus& Facility :: getStatus() const {
    return status;
}

const string Facility :: toString() const {
    string res = "Facility Name: " + name + "\n" + "FacilityStatus: ";
    if (status == FacilityStatus :: UNDER_CONSTRUCTIONS) 
        return res += "UNDER_CONSTRUCTIONS";
    return  res += "OPERATIONAL";
}

FacilityStatus Facility :: step(){
    if(timeLeft == 0)
        status = FacilityStatus :: OPERATIONAL;
    else
        timeLeft--;
    return status;    
}