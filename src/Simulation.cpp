#pragma once
#include "Simulation.h"
#include <fstream>
#include <stdexcept>

Simulation::Simulation(const string &configFilePath):
isRunning(false),
planCounter(0)
{
    actionsLog = vector<BaseAction*>();
    plans= vector<Plan>();
    settlements=vector<Settlement*>();
    facilitiesOptions=vector<FacilityType>();

    std::ifstream file(configFilePath);//open file

    string line;

    while (std::getline(file,line))
    {
        vector<string> args = Auxiliary::parseArguments(line);
        if (args.size()!=0)
        {
            string command = args.at(0);
            if (command=="settlement")
            {
                string name = args.at(1);
                string typeNum = args.at(2);
                SettlementType type;
                if (typeNum=="0")
                    type = SettlementType::VILLAGE;
                else if (typeNum=="1")
                    type = SettlementType::CITY;
                else if (typeNum=="2")
                    type = SettlementType::METROPOLIS;
                Settlement* settlement = new Settlement(name,type);
                addSettlement(settlement);
            }
            else if (command=="facility")
            {
                string name = args.at(1);
                string categoryNum = args.at(2);
                int priceNum = std::stoi(args.at(3));
                int lifeNum = std::stoi(args.at(4));
                int ecoNum = std::stoi(args.at(5));
                int envNum = std::stoi(args.at(6));

                FacilityCategory cat;

                if (categoryNum=="0")
                    cat = FacilityCategory::LIFE_QUALITY;
                else if (categoryNum=="1")
                    cat = FacilityCategory::ECONOMY;
                else if (categoryNum=="2")
                    cat = FacilityCategory::ENVIRONMENT;

                addFacility(FacilityType(name,cat,priceNum,lifeNum,ecoNum,envNum));
            }
            else if (command=="plan")
            {
                string name = args.at(1);
                string policyStr = args.at(2);
                Settlement* settlement;
                for (Settlement* sett:settlements)
                {
                    if (sett->getName()==name)
                    {
                        settlement=sett;
                    }
                }
                
                SelectionPolicy* policy=nullptr;
                if (policyStr=="nve")
                    policy = new NaiveSelection();
                else if (policyStr=="bal")
                    policy = new BalancedSelection(0, 0, 0);
                else if (policyStr=="eco")
                    policy=new EconomySelection();
                else if (policyStr=="env")
                    policy=new SustainabilitySelection();
                
                addPlan(*settlement,policy);
            }
        }
    }
};    

void Simulation::start(){
    std:: cout<<"the simulation has started"<< std::endl;    
    isRunning = true;
    string illeaglComm = "Error, illegal command syntax";        
    while(isRunning){
        string line;
        std::getline(std::cin, line); 
        vector<string> args = Auxiliary::parseArguments(line);
        int argNum = args.size();
        if(argNum != 0){
            string command = args.at(0);
            BaseAction *action = nullptr;
            if(command == "step"){
                if(argNum != 2){
                    std::cout<<illeaglComm<<std::endl;
                } else {
                    int steps = std:: stoi(args.at(1));
                    action = new SimulateStep(steps);
                    action ->act(*this);
                    actionsLog.push_back(action);
                }
            } else if(command == "plan") {
                if(argNum != 3){
                    std::cout<<illeaglComm<<std::endl;
                } else {
                    string name = args.at(1);
                    action = new AddPlan(name, args.at(2));
                    action ->act(*this);
                    actionsLog.push_back(action);
                }                 
            } else if(command == "settlement") {
                if(argNum != 3){
                    std::cout<<illeaglComm<<std::endl;
                } else {
                    try{
                        string name = args.at(1);
                        int type = std:: stoi(args.at(2));
                        SettlementType settlementType;
                        bool error = true;
                        switch (type){
                            case 0:
                                settlementType = SettlementType:: VILLAGE;
                            case 1:
                                settlementType = SettlementType:: CITY;
                            case 2:
                                settlementType = SettlementType:: METROPOLIS;                   
                            default:
                                error = true;
                        }
                        if(error){
                            std::cout<<illeaglComm<<std::endl;        
                        }
                        else{
                            action = new AddSettlement(name,settlementType);
                            action ->act(*this);
                            actionsLog.push_back(action);
                        }
                    } catch(const std:: invalid_argument& e){
                        std::cout<<illeaglComm<<std::endl;
                    }
                }        
            } else if(command == "facility") {
                if(argNum != 7){
                    std::cout<<illeaglComm<<std::endl;
                } else {
                    try{
                        string name = args.at(1);
                        int categoryNum = std:: stoi(args.at(2));
                        bool error = false;
                        FacilityCategory category;
                        switch(categoryNum){
                            case 0:
                                category = FacilityCategory:: LIFE_QUALITY;
                            case 1:
                                category = FacilityCategory:: ECONOMY;
                            case 2:
                                category = FacilityCategory:: ENVIRONMENT;
                            default:
                                error = true;            
                        }
                        if(error){
                            std:: cout<<illeaglComm<<std::endl;
                        } else {
                            int price = std:: stoi(args.at(3));
                            int life = std:: stoi(args.at(4));
                            int eco = std:: stoi(args.at(5));
                            int env = std:: stoi(args.at(6));
                            action = new AddFacility(name, category, price, life, eco, env);
                            action ->act(*this);
                            actionsLog.push_back(action);
                        }
                    } catch(const std:: invalid_argument& e){
                        std::cout<<illeaglComm<<std::endl;
                    }
                }        
            } else if(command == "planStatus") {
                if(argNum != 2){
                    std::cout<<illeaglComm<<std::endl;
                } else {
                    try{
                        int planID = std:: stoi(args.at(1));
                        action = new PrintPlanStatus(planID);
                        action ->act(*this);
                        actionsLog.push_back(action);
                    } catch(const std:: invalid_argument& e){
                        std::cout<<illeaglComm<<std::endl;
                    }
                }
            } else if(command == "changePolicy") {
                if(argNum != 3){
                    std::cout<<illeaglComm<<std::endl;
                }
                try{
                    int planID = std::stoi(args.at(1));
                    action = new ChangePlanPolicy(planID, args.at(2));
                    action->act(*this);
                    actionsLog.push_back(action);
                } catch(const std:: invalid_argument& e){
                    std::cout<<illeaglComm<<std::endl;
                }
            } else if(command == "log") {
                if(argNum != 1)
                    std::cout<<illeaglComm<<std::endl;
                else {
                    action = new PrintActionsLog();
                    action->act(*this);
                    actionsLog.push_back(action);
                }  
            } else if(command == "close") {
                if(argNum != 1)
                    std::cout<<illeaglComm<<std::endl;
                else {
                    action = new Close();
                    action->act(*this);
                    actionsLog.push_back(action);
                }  

            } else if(command == "backup") {
                if(argNum != 1)
                    std::cout<<illeaglComm<<std::endl;
                else {
                    action = new BackupSimulation();
                    action->act(*this);
                    actionsLog.push_back(action);
                }  
            } else if(command == "restore") {
                if(argNum != 1)
                    std::cout<<illeaglComm<<std::endl;
                else {
                    action = new RestoreSimulation();
                    action->act(*this);
                    actionsLog.push_back(action);
                }                        
            } else {
                std::cout<<illeaglComm<<std::endl;                
            }
        }
    }
}

void Simulation :: addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    plans.push_back(Plan(planCounter, settlement, selectionPolicy, vector<FacilityType>()));
    planCounter++;
}

void Simulation :: addAction(BaseAction *action){
    actionsLog.push_back(action);
}

bool Simulation :: addSettlement(Settlement *settlement){
    settlements.push_back(settlement);
    return true;//why bool?
}

bool Simulation :: addFacility(FacilityType facility){
    facilitiesOptions.push_back(facility);
    return true;    
}

bool Simulation :: isSettlementExists(const string &settlementName){
    bool found = false;
    for(int i = 0; i < settlements.size() & !found; i++){
        if(settlements.at(i)->getName() == settlementName)
            found = true;
    }
    return found;
}

Settlement& Simulation :: getSettlement(const string &settlementName){
    Settlement *toReturn = nullptr;
    for(int i = 0; i < settlements.size(); i++){
        if(settlements.at(i)->getName() == settlementName){
            toReturn = settlements.at(i);
        }
    }
    return *toReturn;
}

Plan & Simulation :: getPlan(const int planID){
    Plan *toReturn = nullptr;
    if(planID < plans.size() && planID >= 0)
        *toReturn = plans.at(planID);
    return *toReturn;
}

void Simulation :: step() {
    for(Plan plan : plans){
        plan.step();
    }
}

void Simulation :: close(){
    isRunning = false;
}

void Simulation :: open(){
    isRunning = true;
}

vector<BaseAction*>& Simulation:: getActionsLog(){
    return actionsLog;
}

Simulation:: Simulation(Simulation& other):isRunning(other.isRunning),planCounter(other.planCounter){
    actionsLog = vector<BaseAction*>();
    for(BaseAction *action: other.actionsLog){
        actionsLog.push_back(action->clone());
    }
    plans = vector<Plan>();
    for(Plan plan: other.plans){
        plans.push_back(Plan(plan));
    }
    settlements = vector<Settlement *>();
    for(Settlement *set: other.settlements){
        Settlement *newVal = nullptr;
        *newVal = *set;
        settlements.push_back(newVal);
    }
    facilitiesOptions = vector<FacilityType>();
    for(FacilityType type: other.facilitiesOptions){
        facilitiesOptions.push_back(type);
    }
}

Simulation& Simulation :: operator=(const Simulation &other){
    Simulation::~Simulation();
    isRunning = other.isRunning;
    planCounter = other.planCounter;
    actionsLog = vector<BaseAction*>();
    for(BaseAction *action: other.actionsLog){
        actionsLog.push_back(action->clone());
    }
    plans = vector<Plan>();
    for(const Plan &plan: other.plans){
        plans.push_back(Plan(plan));
    }
    settlements = vector<Settlement *>();
    for(Settlement *set: other.settlements){
        Settlement *newVal = nullptr;
        *newVal = *set;
        settlements.push_back(newVal);
    }
    facilitiesOptions = vector<FacilityType>();
    for(FacilityType type: other.facilitiesOptions){
        facilitiesOptions.push_back(type);
    }
}
Simulation ::~Simulation(){
    for(BaseAction *action: actionsLog){
        delete action;
    }
    for(Settlement *set: settlements){
        delete set;
    }
}
Simulation :: Simulation(Simulation&& other):isRunning(other.isRunning),planCounter(other.planCounter){
    actionsLog = other.actionsLog;
    plans = other.plans;    
    settlements = other.settlements;
    facilitiesOptions = other.facilitiesOptions;
    other.facilitiesOptions = vector<FacilityType>();
    other.settlements = vector<Settlement *>();
    other.plans = vector<Plan>();
    other.actionsLog = vector<BaseAction *>();
}
Simulation& Simulation :: operator=(Simulation&& other){
    Simulation::~Simulation();
    isRunning = other.isRunning;
    planCounter = other.planCounter;
    actionsLog = other.actionsLog;
    plans = other.plans;    
    settlements = other.settlements;
    facilitiesOptions = other.facilitiesOptions;
    other.facilitiesOptions = vector<FacilityType>();
    other.settlements = vector<Settlement *>();
    other.plans = vector<Plan>();
    other.actionsLog = vector<BaseAction *>();    
}