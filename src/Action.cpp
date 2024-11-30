#include "Action.h"
#include "Plan.h"
#include <iostream>

extern Simulation* backup=nullptr;


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
    string statusStr = "";
    if (getStatus() == ActionStatus::COMPLETED)
        statusStr="COMPLETED";
    else if (getStatus()==ActionStatus::ERROR)
        statusStr=="ERROR";
    return "step "+std::to_string(numOfSteps)+" "+statusStr;
}



AddPlan::AddPlan(const string &newSettlementName, const string &newSelectionPolicy)
:settlementName(newSettlementName)
,selectionPolicy(newSelectionPolicy)
{}
void AddPlan::act(Simulation& simulation)
{
    if (!simulation.isSettlementExists(settlementName))
        error("Cannot create this plan");
    else{
        Settlement settlement = simulation.getSettlement(settlementName);
        bool policyExist = true;
        SelectionPolicy* policy;
        if (selectionPolicy=="nve")
            policy = new NaiveSelection();
        else if (selectionPolicy=="bal")
            policy = new BalancedSelection(0,0,0);
        else if (selectionPolicy=="eco")
            policy = new EconomySelection();
        else if (selectionPolicy=="env")
            policy = new SustainabilitySelection();
        else
        {
            error("Cannot create this plan");
            policyExist=false;
        }
        if (policyExist)
            {
            simulation.addPlan(settlement,policy);
            complete();
            }   
        }

}
const string AddPlan::toString() const
{
    string statusStr = "";
    if (getStatus() == ActionStatus::COMPLETED)
        statusStr="COMPLETED";
    else if (getStatus()==ActionStatus::ERROR)
        statusStr=="ERROR";
    return "plan "+settlementName+" "+selectionPolicy+" "+statusStr;
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
    string typeStr = "";
    if (settlementType==SettlementType::VILLAGE)
        typeStr = "0";
    else if (settlementType==SettlementType::CITY)
        typeStr="1";
    else if (settlementType==SettlementType::METROPOLIS)
        typeStr="2";

    
    string statusStr = "";
    if (getStatus() == ActionStatus::COMPLETED)
        statusStr="COMPLETED";
    else if (getStatus()==ActionStatus::ERROR)
        statusStr=="ERROR";

    return "settlement "+settlementName+" "+typeStr+" "+statusStr;
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
const string AddFacility::toString() const
{
    string catStr = "";
    if (facilityCategory==FacilityCategory::LIFE_QUALITY)
        catStr="0";
    else if (facilityCategory==FacilityCategory::ECONOMY)
        catStr = "1";
    else if (facilityCategory==FacilityCategory::ENVIRONMENT)
        catStr="2";

    string statusStr = "";
    if (getStatus() == ActionStatus::COMPLETED)
        statusStr="COMPLETED";
    else if (getStatus()==ActionStatus::ERROR)
        statusStr=="ERROR";
    return "SimulateStep "+facilityName+" "+catStr+" "+std::to_string(price)+" "+std::to_string(lifeQualityScore)+" "+std::to_string(economyScore)+" "+std::to_string(environmentScore) +" "+statusStr;
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
    string statusStr = "";
    if (getStatus() == ActionStatus::COMPLETED)
        statusStr="COMPLETED";
    else if (getStatus()==ActionStatus::ERROR)
        statusStr=="ERROR";
    return "planStatus "+std::to_string(planId)+" "+statusStr;
}



ChangePlanPolicy::ChangePlanPolicy(const int newPlanId, const string &newNewPolicy)
:planId(newPlanId)
,newPolicy(newNewPolicy)
{}
void ChangePlanPolicy::act(Simulation& simulation)
{
   if (simulation.isPlanExist(planId))
        error("Cannot change selection policy");
    else{
    Plan plan = simulation.getPlan(planId);
    SelectionPolicy* policy;
    if (newPolicy=="nve")
        policy = new NaiveSelection();
    else if (newPolicy=="bal")
        policy = new BalancedSelection(plan.getlifeQualityScore(),plan.getEconomyScore(),plan.getEnvironmentScore());
    else if (newPolicy=="eco")
        policy = new EconomySelection();
    else if (newPolicy=="env")
        policy = new SustainabilitySelection();
    if (typeid(simulation.getPlan(planId).getSelectionPolicy())==typeid(policy))
        error("Cannot change selection policy");
    else{
        plan.setSelectionPolicy(policy);
        complete();
      }
    }
}
ChangePlanPolicy* ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}
const string ChangePlanPolicy::toString() const
{
    return "changePolicy "+std::to_string(planId)+" "+newPolicy+" to policy "+newPolicy;
}



PrintActionsLog::PrintActionsLog()
{}
void PrintActionsLog::act(Simulation& simulation)
{
    vector<BaseAction*>& actionsLog = simulation.getActionsLog();
    for (int i=0;i<actionsLog.size();i++)
    {
        BaseAction* currentAction = actionsLog.at(i);
        std::cout<<currentAction->toString()<<std::endl;
    }
    complete();
}
PrintActionsLog* PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}
const string PrintActionsLog::toString() const
{
        string statusStr = "";
    if (getStatus() == ActionStatus::COMPLETED)
        statusStr="COMPLETED";
    else if (getStatus()==ActionStatus::ERROR)
        statusStr=="ERROR";
    return "log "+statusStr;
}

Close::Close()
{}
void Close::act(Simulation& simulation)
{
    simulation.close();
    int i=0;
    while(simulation.isPlanExist(i))
    {
        PrintPlanStatus* pps = new PrintPlanStatus(i);
        pps->act(simulation);
    }
}



BackupSimulation::BackupSimulation()
{}
void BackupSimulation::act(Simulation& simulation)
{
    backup = new Simulation(simulation);
    complete();
}
BackupSimulation* BackupSimulation::clone() const
{
    return new BackupSimulation(*this);
}
const string BackupSimulation::toString() const
{
        string statusStr = "";
    if (getStatus() == ActionStatus::COMPLETED)
        statusStr="COMPLETED";
    else if (getStatus()==ActionStatus::ERROR)
        statusStr=="ERROR";
    return "backup "+statusStr;
}


RestoreSimulation::RestoreSimulation()
{}
void RestoreSimulation::act(Simulation& simulation)
{
    if (backup==nullptr)
        error("No backup available");
    else{
        simulation = *backup;
        complete();
    }
}
RestoreSimulation* RestoreSimulation::clone() const 
{
    return new RestoreSimulation(*this);
}
const string RestoreSimulation::toString() const
{
        string statusStr = "";
    if (getStatus() == ActionStatus::COMPLETED)
        statusStr="COMPLETED";
    else if (getStatus()==ActionStatus::ERROR)
        statusStr=="ERROR";
    return "restore "+statusStr;
}