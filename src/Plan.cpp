#include "Plan.h"
#include <iostream>

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
    switch(newSettlement.getType()) // set construction limit according to the settlement type
    {
        case SettlementType::VILLAGE : constructionLimit = 1;
        break;
        case SettlementType::CITY : constructionLimit = 2;
        break;
        case SettlementType::METROPOLIS : constructionLimit = 3;
        break;
        default: constructionLimit = 0; //should never happen
        break;
    }
};

const int Plan::getEconomyScore() const{return economy_score;};
const int Plan::getEnvironmentScore() const{return environment_score;};
const int Plan::getlifeQualityScore() const{return life_quality_score;};

void Plan::setSelectionPolicy(SelectionPolicy *newSelectionPolicy)
{
    selectionPolicy=newSelectionPolicy;
}

void Plan::step()
{   //step 1
    if(status==PlanStatus::AVALIABLE)
    {
        //step 2
        while (constructionLimit<underConstruction.size())
        {
            FacilityType selectedType = selectionPolicy->selectFacility(facilityOptions);
            Facility* newFacility = new Facility(selectedType, settlement.getName());
            underConstruction.push_back(newFacility);
        }

        //step 3

        //step 4
        if (underConstruction.size()==constructionLimit)
            {status = PlanStatus::BUSY;}
        else
        {status = PlanStatus::AVALIABLE;}
        
    }
};

void Plan::printStatus()
{
    switch(status)
    {
        case PlanStatus::AVALIABLE : std::cout << "Available";
        break;
        case PlanStatus::BUSY : std::cout << "Busy";
        default : std::cout << "status undefined";

    }
}

void Plan::addFacility(Facility* newFacility)
{
    facilities.push_back(newFacility);
};

const string Plan::toString() const{return "Plan "+plan_id;}

const vector<Facility*>& Plan::getFacilities() const{return facilities;};
