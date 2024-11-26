#pragma once
#include "SelectionPolicy.h"
NaiveSelection :: NaiveSelection() : lastSelectedIndex(-1){}
const FacilityType& NaiveSelection :: selectFacility(const vector<FacilityType>& facilitiesOptions){
    int currIndex = lastSelectedIndex + 1;
    FacilityType *toReturn = nullptr;
    if(facilitiesOptions.size() > currIndex){
        *toReturn = facilitiesOptions.at(currIndex);
        lastSelectedIndex = currIndex;
    }
    return *toReturn;
}

const string NaiveSelection :: toString() const{
    return "selection policy: nev";
}

NaiveSelection * NaiveSelection :: clone() const{
    return new NaiveSelection();
}

NaiveSelection::~NaiveSelection()
{
}

BalancedSelection :: BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : LifeQualityScore(LifeQualityScore),
EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){};

int BalancedSelection :: getMaxDistance(FacilityType & facility){
    return std::max(facility.getEconomyScore() + EconomyScore, facility.getEnvironmentScore() + EnvironmentScore, facility.getLifeQualityScore() + LifeQualityScore);
}

const FacilityType& BalancedSelection :: selectFacility(const vector<FacilityType>& facilitiesOptions){
    FacilityType *minChoice = nullptr;
    if(facilitiesOptions.size() > 0){
        *minChoice = facilitiesOptions.at(0);
        int maxDistanceInmin = getMaxDistance(*minChoice);
        for(FacilityType currFacility : facilitiesOptions){
            int currDistance = getMaxDistance(currFacility);
            if(currDistance < maxDistanceInmin){
                *minChoice = currFacility;
                maxDistanceInmin = currDistance;
            }
        }
        //possibly redundant
        EconomyScore += minChoice->getEconomyScore();
        EnvironmentScore += minChoice->getEnvironmentScore();
        LifeQualityScore += minChoice->getLifeQualityScore();
    }
    return *minChoice;
}    

const string BalancedSelection :: toString() const{
    return "selection policy: bal";
}

BalancedSelection * BalancedSelection :: clone() const{
    return new BalancedSelection(LifeQualityScore, EconomyScore, EnvironmentScore);
}

BalancedSelection::~BalancedSelection()
{
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
                *toReturn = facilitiesOptions.at(currIndex);
            }
        }
    }
    lastSelectedIndex = currIndex;
    return *toReturn;
}

const string EconomySelection :: toString() const{
    return "selection policy: eco";
}

EconomySelection * EconomySelection :: clone() const{
    return new EconomySelection();
}

EconomySelection::~EconomySelection()
{
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
                *toReturn = facilitiesOptions.at(currIndex);
            }
        }
    }
    lastSelectedIndex = currIndex;
    return *toReturn;
}

const string SustainabilitySelection :: toString() const{
    return "selection policy: env";
}

SustainabilitySelection * SustainabilitySelection :: clone() const{
    return new SustainabilitySelection();
}

SustainabilitySelection::~SustainabilitySelection()
{
}