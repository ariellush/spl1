#include "Plan.h"

Plan::Plan(const int planId, const Settlement &newSettlement, SelectionPolicy *newSelectionPolicy, const vector<FacilityType> &newFacilityOptions) 
:plan_id(plan_id),
settlement(const_cast<Settlement&>(newSettlement)) 
,selectionPolicy(newSelectionPolicy)
,facilityOptions(newFacilityOptions),
life_quality_score(0),
economy_score(0),
environment_score(0)
{
    facilities=vector<Facility*>();
    underConstruction=vector<Facility*>();
};

const int Plan::getEconomyScore() const{return economy_score;};
const int Plan::getEnvironmentScore() const{return environment_score;};
const int Plan::getlifeQualityScore() const{return life_quality_score;};


