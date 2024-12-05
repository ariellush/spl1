#pragma once
#include "SelectionPolicy.h"
NaiveSelection :: NaiveSelection() : lastSelectedIndex(-1){}
const FacilityType& NaiveSelection :: selectFacility(const vector<FacilityType>& facilitiesOptions){
    int currIndex = lastSelectedIndex + 1;
    if(facilitiesOptions.size() > currIndex){
        lastSelectedIndex = currIndex;
    }
    return facilitiesOptions.at(currIndex);
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

int BalancedSelection :: getMaxDistance(const FacilityType & facility){
    int maxDis = std::max(std::max(facility.getEconomyScore() + EconomyScore, facility.getEnvironmentScore() + EnvironmentScore), facility.getLifeQualityScore() + LifeQualityScore);
    int minDis = std::min(std::min(facility.getEconomyScore() + EconomyScore, facility.getEnvironmentScore() + EnvironmentScore), facility.getLifeQualityScore() + LifeQualityScore); 
    return maxDis - minDis;
}

const FacilityType& BalancedSelection :: selectFacility(const vector<FacilityType>& facilitiesOptions){
    int index = 0;
    int maxDistanceInmin = getMaxDistance(facilitiesOptions.at(0));
    for(int i = 1; i < facilitiesOptions.size(); i++){
        int currDistance = getMaxDistance(facilitiesOptions.at(i));
        if(currDistance < maxDistanceInmin){
            index = i;
            maxDistanceInmin = currDistance;
        }
    }
    //possibly redundant
    EconomyScore += facilitiesOptions.at(index).getEconomyScore();
    EnvironmentScore += facilitiesOptions.at(index).getEnvironmentScore();
    LifeQualityScore += facilitiesOptions.at(index).getLifeQualityScore();
    return facilitiesOptions.at(index);
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
    if(facilitiesOptions.size() > currIndex){
        bool ecoFound = false;
        for(;currIndex < facilitiesOptions.size() & !ecoFound; currIndex++){
            if(facilitiesOptions.at(currIndex).getCategory() == FacilityCategory :: ECONOMY){
                ecoFound = true;
                currIndex--;
            }
        }
    }
    lastSelectedIndex = currIndex-1;
    return facilitiesOptions.at(currIndex);
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
    if(facilitiesOptions.size() > currIndex){
        bool lifeFound = false;
        for(;currIndex < facilitiesOptions.size() & !lifeFound; currIndex++){
            if(facilitiesOptions.at(currIndex).getCategory() == FacilityCategory :: LIFE_QUALITY){
                lifeFound = true;
                currIndex--;
            }
        }
    }
    lastSelectedIndex = currIndex-1;
    return facilitiesOptions.at(currIndex);
}

const string SustainabilitySelection :: toString() const{
    return "env";
}

SustainabilitySelection * SustainabilitySelection :: clone() const{
    SustainabilitySelection * toReturn = new SustainabilitySelection();
    (*toReturn).lastSelectedIndex = lastSelectedIndex;
    return toReturn;
}
