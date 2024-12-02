#pragma once
#include "SelectionPolicy.h"
NaiveSelection :: NaiveSelection() : lastSelectedIndex(-1){}
const FacilityType& NaiveSelection :: selectFacility(const vector<FacilityType>& facilitiesOptions){
    int currIndex = lastSelectedIndex + 1;
    FacilityType *toReturn = nullptr;
    if(facilitiesOptions.size() > currIndex){
        toReturn = new FacilityType(std::move(facilitiesOptions.at(currIndex)));
        lastSelectedIndex = currIndex;
    }
    return *toReturn;
}

const string NaiveSelection :: toString() const{
    return "nev";
}

NaiveSelection * NaiveSelection :: clone() const{
    NaiveSelection * toReturn = new NaiveSelection();
    (*toReturn).lastSelectedIndex = lastSelectedIndex;
    return toReturn;
}

BalancedSelection :: BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : LifeQualityScore(LifeQualityScore),
EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){};

int BalancedSelection :: getMaxDistance(FacilityType & facility){
    int maxDis = std::max(std::max(facility.getEconomyScore() + EconomyScore, facility.getEnvironmentScore() + EnvironmentScore), facility.getLifeQualityScore() + LifeQualityScore);
    int minDis = std::min(std::min(facility.getEconomyScore() + EconomyScore, facility.getEnvironmentScore() + EnvironmentScore), facility.getLifeQualityScore() + LifeQualityScore); 
    return maxDis - minDis;
}

const FacilityType& BalancedSelection :: selectFacility(const vector<FacilityType>& facilitiesOptions){
    FacilityType *toReturn = nullptr;
    if(facilitiesOptions.size() > 0){
        toReturn = new FacilityType(std::move(facilitiesOptions.at(0))); 
        int maxDistanceInmin = getMaxDistance(*toReturn);
        for(FacilityType currFacility : facilitiesOptions){
            int currDistance = getMaxDistance(currFacility);
            if(currDistance < maxDistanceInmin){
                if(toReturn != nullptr)
                    delete(toReturn);
                toReturn = new FacilityType(std::move(currFacility));
                maxDistanceInmin = currDistance;
            }
        }
        //possibly redundant
        EconomyScore += toReturn->getEconomyScore();
        EnvironmentScore += toReturn->getEnvironmentScore();
        LifeQualityScore += toReturn->getLifeQualityScore();
    }
    return *toReturn;
}    

const string BalancedSelection :: toString() const{
    return "bal";
}

BalancedSelection * BalancedSelection :: clone() const{
    return new BalancedSelection(LifeQualityScore, EconomyScore, EnvironmentScore);
}

EconomySelection :: EconomySelection() : lastSelectedIndex(-1){}
const FacilityType& EconomySelection :: selectFacility(const vector<FacilityType>& facilitiesOptions){
    int currIndex = lastSelectedIndex + 1;
    FacilityType *toReturn = nullptr;
    if(facilitiesOptions.size() > currIndex){
        bool ecoFound = false;
        for(;currIndex < facilitiesOptions.size() & !ecoFound; currIndex++){
            if(facilitiesOptions.at(currIndex).getCategory() == FacilityCategory :: ECONOMY){
                ecoFound = true;
                toReturn = new FacilityType(std::move(facilitiesOptions.at(currIndex))); 
            }
        }
    }
    lastSelectedIndex = currIndex;
    return *toReturn;
}

const string EconomySelection :: toString() const{
    return "eco";
}

EconomySelection * EconomySelection :: clone() const{
    EconomySelection * toReturn = new EconomySelection();
    (*toReturn).lastSelectedIndex = lastSelectedIndex;
    return toReturn;
}

SustainabilitySelection :: SustainabilitySelection() : lastSelectedIndex(-1){}
const FacilityType& SustainabilitySelection :: selectFacility(const vector<FacilityType>& facilitiesOptions){
    int currIndex = lastSelectedIndex + 1;
    FacilityType *toReturn = nullptr;
    if(facilitiesOptions.size() > currIndex){
        bool lifeFound = false;
        for(;currIndex < facilitiesOptions.size() & !lifeFound; currIndex++){
            if(facilitiesOptions.at(currIndex).getCategory() == FacilityCategory :: LIFE_QUALITY){
                lifeFound = true;
                toReturn = new FacilityType(std::move(facilitiesOptions.at(currIndex))); 
            }
        }
    }
    lastSelectedIndex = currIndex;
    return *toReturn;
}

const string SustainabilitySelection :: toString() const{
    return "env";
}

SustainabilitySelection * SustainabilitySelection :: clone() const{
    SustainabilitySelection * toReturn = new SustainabilitySelection();
    (*toReturn).lastSelectedIndex = lastSelectedIndex;
    return toReturn;
}
