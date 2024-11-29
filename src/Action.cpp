#include "Action.h"
#include "Plan.h"
#include <iostream>


ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string newErrorMsg)
{
    status = ActionStatus::ERROR;
    errorMsg=newErrorMsg;
}
const string& BaseAction::getErrorMsg() const
{
    return errorMsg;
}



SimulateStep::SimulateStep(const int stepsNum):numOfSteps(stepsNum)
{}
void SimulateStep::act(Simulation& Simulation)
{
    for (int i=0;i<numOfSteps;i++)
    {
        Simulation.step();
    }
    complete();
}
SimulateStep* SimulateStep::clone() const
{
    return new SimulateStep(*this);
}
const string SimulateStep::toString() const
{
    return "SimulateStep "+std::to_string(numOfSteps) +" steps";
}



AddPlan::AddPlan(const string &newSettlementName, const string &newSelectionPolicy)
:settlementName(newSettlementName)
,selectionPolicy(newSelectionPolicy)
{}
void AddPlan::act(Simulation& simulation)
{
    Settlement settlement = simulation.getSettlement(settlementName);
    //error
    SelectionPolicy* policy;
    if (selectionPolicy=="nve")
        policy = new NaiveSelection();
    else if (selectionPolicy=="bal")
        policy = new BalancedSelection(0,0,0); //change that
    else if (selectionPolicy=="eco")
        policy = new EconomySelection();
    else if (selectionPolicy=="env")
        policy = new SustainabilitySelection();
    else
        error("Cannot create this plan");
    if (getErrorMsg()!="Cannot create this plan")
    {
        simulation.addPlan(settlement,policy);
        complete();
    }

}
const string AddPlan::toString() const
{
    return "AddPlan for settlement "+settlementName+" with "+selectionPolicy +" policy";
}
AddPlan* AddPlan::clone() const 
{
    return new AddPlan(*this);
}



AddSettlement::AddSettlement(const string &newSettlementName,SettlementType newSettlementType)
:settlementName(newSettlementName)
,settlementType(newSettlementType)
{}
void AddSettlement::act (Simulation& simulation)
{
    if (simulation.isSettlementExists(settlementName))
        error("Settlement already exist");
    else{
        Settlement* settlement = new Settlement(settlementName,settlementType);
        simulation.addSettlement(settlement);
        complete();
    }
}
AddSettlement* AddSettlement::clone() const
{
    return new AddSettlement(*this);
}
const string AddSettlement::toString() const
{
    if (settlementType==SettlementType::VILLAGE)
        return "AddSettlement "+settlementName+" of type VILLAGE";
    else if (settlementType==SettlementType::CITY)
        return "AddSettlement "+settlementName+" of type CITY";
    else if (settlementType==SettlementType::METROPOLIS)
        return "AddSettlement "+settlementName+" of type METROPOLIS";
    else
        return "AddSettlement "+settlementName+" of type UNDEFINED";//should never happen
    
}



AddFacility::AddFacility(const string &newFacilityName, const FacilityCategory newFacilityCategory, const int newPrice, const int newLifeQualityScore, const int newEconomyScore, const int newEnvironmentScore)
:facilityName(newFacilityName)
,facilityCategory(newFacilityCategory)
,price(newPrice)
,lifeQualityScore(newLifeQualityScore)
,economyScore(newEconomyScore)
,environmentScore(newEnvironmentScore)
{}
void AddFacility::act(Simulation& simulation)
{
    /*if (simulation.isFacilityExist(facilityName)) //Add that
        error("Facility already exists");
    else{*/
    FacilityType newFacility = FacilityType(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore);
    simulation.addFacility(newFacility);
    /*}*/
}
AddFacility* AddFacility::clone() const
{
    return new AddFacility(*this);
}


PrintPlanStatus::PrintPlanStatus(int newPlanId):planId(newPlanId)
{}
void PrintPlanStatus::act(Simulation &simulation)
{

    Plan& plan = simulation.getPlan(planId);
    if (&plan==nullptr)
     error("Plan doesn't exist");
    std::cout<<"Plan id: "<<planId << "settlementName"<<std::endl;//get name
    std::cout<<"planStatus: "<<plan.getPlanStatusStr()<<std::endl;
    std::cout<<"LifeQualityScore: "<<plan.getlifeQualityScore()<<std::endl;
    std::cout<<"EconomyScore: "<<plan.getEconomyScore()<<std::endl;
    std::cout<<"EnvironmentScore: "<<plan.getEnvironmentScore()<<std::endl;
    const vector<Facility*> facilities = plan.getFacilities();
    for (int i=0;i<facilities.size();i++)
    {
        std::cout<<"facilityName: "<<facilities.at(i)->getName()<<" facilityStatus: "<</*facilities.at(i)->getStatus()<<*/std::endl;
    }

    complete();
}
PrintPlanStatus* PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(*this);
}
const string PrintPlanStatus::toString() const
{
    return "Print plan status for planID: "+std::to_string(planId);
}
