#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Auxiliary.h"
#include "Action.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        void step();
        void close();
        void open();
        Simulation(Simulation& other);
        Simulation& operator=(const Simulation &other);
        ~Simulation();
        Simulation(Simulation&& other);
        Simulation& operator=(Simulation&& other);
        vector<BaseAction*>& getActionsLog();
        bool isPlanExist(int planId) const;
        bool isFacilityExist(string facilityName) const;

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

};