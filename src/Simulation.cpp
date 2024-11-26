#pragma once
#include "Simulation.h"
#include "Auxiliary.h"

#include "Settlement.h"
#include "Facility.h"

#include <fstream>

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
                    policy = new BalancedSelection();
                else if (policyStr=="eco")
                    policy=new EconomySelection();
                else if (policyStr=="env")
                    policy=new SustainabilitySelection();
                
                addPlan(*settlement,policy);
                
            }
        }
    }




#hiiiiiii
}