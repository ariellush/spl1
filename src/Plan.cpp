#include "Plan.h"
#include <iostream>

Plan::Plan(const int planId, const Settlement &newSettlement, SelectionPolicy *newSelectionPolicy, const vector<FacilityType> &newFacilityOptions) 
:plan_id(planId),
settlement(newSettlement) 
,selectionPolicy(newSelectionPolicy)
,facilityOptions(newFacilityOptions),
life_quality_score(0),
economy_score(0),
environment_score(0),
facilities(vector<Facility*>()),
underConstruction(vector<Facility*>()),
status(PlanStatus::AVALIABLE)
{
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

//copy constructor
//create an identicle (yet seperate) copy of other
//essencialy the same plan up to that point, refering to the same settlement with the same
//buliding under construction and constructed
//allowes to change the copy of the plan without discarding to original
Plan::Plan(Plan& other)
:plan_id(other.plan_id)
,settlement(other.settlement)
,selectionPolicy(other.selectionPolicy->clone())
,facilityOptions(other.facilityOptions),
life_quality_score(other.life_quality_score)
,economy_score(other.economy_score)
,environment_score(other.environment_score)
,facilities(other.facilities)
,underConstruction(other.underConstruction)
,constructionLimit(other.constructionLimit)
,status(other.status)
{};

Plan::Plan(const Plan& other)
:plan_id(other.plan_id)
,settlement(other.settlement)
,selectionPolicy(other.selectionPolicy->clone())
,facilityOptions(other.facilityOptions),
life_quality_score(other.life_quality_score)
,economy_score(other.economy_score)
,environment_score(other.environment_score)
,facilities(other.facilities)
,underConstruction(other.underConstruction)
,constructionLimit(other.constructionLimit)
,status(other.status)
{};

Plan::~Plan(){
    delete selectionPolicy;
}

//essencialy only copies the selection policy 
//since the settlement cannot be changed
Plan& Plan::operator=(const Plan &other)
{
    delete selectionPolicy;
    if (&other != this)
    {
        selectionPolicy= other.selectionPolicy->clone();
    }
}

//Move constructor
Plan::Plan(Plan&& other)
:plan_id(other.plan_id)
,settlement(other.settlement)
,selectionPolicy(other.selectionPolicy)
,facilityOptions(other.facilityOptions),
life_quality_score(other.life_quality_score)
,economy_score(other.economy_score)
,environment_score(other.environment_score)
,facilities(other.facilities)
,underConstruction(other.underConstruction)
,constructionLimit(other.constructionLimit)
,status(other.status)
{
    other.selectionPolicy=nullptr;
}


//Move assignment operator
Plan& Plan::operator=(Plan&& other)
{
    selectionPolicy=other.selectionPolicy;
    other.selectionPolicy=nullptr;
}

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
        while (constructionLimit>underConstruction.size())
        {
            selectionPolicy->selectFacility(facilityOptions);
            FacilityType selectedType(selectionPolicy->selectFacility(facilityOptions));
            Facility* newFacility = new Facility(selectedType, settlement.getName());
            underConstruction.push_back(newFacility);
        }
    }
    //step 3
    for(int i=underConstruction.size()-1;i>=0;i--)
    {
        underConstruction[i]->setStatus(underConstruction[i]->step());
        if (underConstruction[i]->getStatus()==FacilityStatus::OPERATIONAL)
        {
            facilities.push_back(underConstruction[i]);
            this->economy_score =  this->environment_score +  underConstruction[i]->getEconomyScore();
            this->environment_score =  this->environment_score + underConstruction[i]->getEnvironmentScore();
            this->life_quality_score = this->life_quality_score + underConstruction[i]->getLifeQualityScore();
            underConstruction.erase(underConstruction.begin()+i);
        }

    }
    //step 4
    if (underConstruction.size()==constructionLimit)
        {status = PlanStatus::BUSY;}
    else
    {status = PlanStatus::AVALIABLE;}
        
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
const vector<Facility *>& Plan :: getUnderConstruction() const{return underConstruction;};


string Plan::getPlanStatusStr() const
{
    if (status==PlanStatus::AVALIABLE)
        return "AVALIABLE";
    else if (status==PlanStatus::BUSY)
        return "BUSY";
    else
        return "UNDEFINED";//should never happen
}

SelectionPolicy* Plan::getSelectionPolicy() const
{
    return selectionPolicy;
}

int Plan:: getID() const{
    return plan_id;
}

const string& Plan::getSettlementName() const
{
    return settlement.getName();
}