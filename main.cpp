#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>
#include <Stop.h>
#include <Route.h>

using namespace std;

class Bus {
private:
    int currentPassengers;
protected:
    string busNumber;
    string busType;
    int capacity;
    double fareRate; // per km fare rate
    double minimumFare;
    double maximumFare;
    Route* assignedRoute;
    int currentStopIndex;
    int maxOvercrowding;  // Maximum passengers, including those standing (overcrowding)

public:
    Bus(string busNum, string busTyp, int cap, double fareRat, double minFare, double maxFare)
        :
        busNumber(busNum),
        busType(busTyp),
        capacity(cap), maxOvercrowding(cap), currentPassengers(0), currentStopIndex(0),
        fareRate(fareRat), minimumFare(minFare), maximumFare(maxFare),
        assignedRoute(nullptr) {
    }

    virtual ~Bus() {}

    virtual double calculateFare(double distance) const = 0;

    virtual void move() {
        if (assignedRoute == nullptr) return;
        const auto& stops = assignedRoute->getStops();
        currentStopIndex = (currentStopIndex + 1) % stops.size();
    }

    virtual Stop* getCurrentStop() const {
        if (!assignedRoute) return nullptr;
        return assignedRoute->getStops()[currentStopIndex];
    }

    virtual bool canBoardPassenger() const {
        return currentPassengers < maxOvercrowding;
    }

    virtual bool boardPassenger() {
        if (canBoardPassenger()) {
            ++currentPassengers;
            return true;
        }
        return false;
    }

    virtual void displayBasicInfo() const {
        cout << "=================================\n";
        cout << "Bus Number: " << busNumber << "\n";
        cout << "Type: " << busType << "\n";
        cout << "Capacity: " << capacity << " passengers\n";
        cout << "Current Passengers: " << currentPassengers << "\n";
        cout << "Fare Rate: Rs " << fareRate << "/km\n";
        cout << "Minimum Fare: Rs " << minimumFare << "\n";
        cout << "Maximum Fare: Rs " << maximumFare << "\n";
        cout << "Max Overcrowding Limit: " << maxOvercrowding << " passengers\n";
        if (isOvercrowded()) {
            cout << "OVERCROWDING STATUS: ACTIVE\n";
        }
        else {
            cout << "OVERCROWDING STATUS: NORMAL\n";
        }
        cout << "=================================\n";
    }

    virtual void assignRoute(Route* route) {
        assignedRoute = route;
    }

    virtual bool setOvercrowdingLimit(int maxLimit) {
        if (maxLimit < capacity) return false;
        maxOvercrowding = maxLimit;
        return true;
    }

    virtual bool isOvercrowded() const {
        return currentPassengers > capacity;
    }

    string getBusNumber() const { return busNumber; }
    string getBusType() const { return busType; }
    int getCapacity() const { return capacity; }
    int getMaxOvercrowding() const { return maxOvercrowding; }
    int getCurrentPassengers() const { return currentPassengers; }
    double getFareRate() const { return fareRate; }
    double getMinimumFare() const { return minimumFare; }
    double getMaximumFare() const { return maximumFare; }
    Route* getAssignedRoute() const { return assignedRoute; }
};

class MiniBus : public Bus {
public:
    MiniBus(string busNum)
        : Bus(busNum, "Minibus", 26, 3, 50, 100) {
        setOvercrowdingLimit(26 + 45);
    }

    double calculateFare(double distance) const override {
        double fare = min(maximumFare, max(minimumFare, distance * fareRate));
        return fare;
    }
};

class ElectricBus : public Bus {
public:
    ElectricBus(string busNum)
        : Bus(busNum, "Electric bus", 32, 5, 80, 120) {
        setOvercrowdingLimit(40 + 40);
    }

    double calculateFare(double distance) const override {
        double fare = min(maximumFare, max(minimumFare, distance * fareRate));
        return fare;
    }
};

class DoubleDecker : public Bus {
public:
    DoubleDecker(string busNum)
        : Bus(busNum, "Double-Decker bus", 80, 7, 100, 150) {
        setOvercrowdingLimit(80 + 40);
    }

    double calculateFare(double distance) const override {
        double fare = min(maximumFare, max(minimumFare, distance * fareRate));
        return fare;
    }
};

enum class JourneyState { WAITING_FOR_PICKUP, ON_BOARD, COMPLETED };

class Ticket {
private:
    Bus* bookedBus;
    Stop* pickup;
    Stop* dropoff;
    JourneyState currentState;

public:
    Ticket(Bus* b, Stop* p, Stop* d) 
        : bookedBus(b), pickup(p), dropoff(d), currentState(JourneyState::WAITING_FOR_PICKUP) {}

    Bus* getBus() const { return bookedBus; }
    JourneyState getState() const { return currentState; }
    void setBoarded() { currentState = JourneyState::ON_BOARD; }
    void setCompleted() { currentState = JourneyState::COMPLETED; }

    bool isAtPickup() const {
        return bookedBus->getCurrentStop() == pickup;
    }

    bool isAtDestination() const {
        return bookedBus->getCurrentStop() == dropoff;
    }
};

void initializeMasterStops(vector<Stop>& masterStops) {
    masterStops.reserve(100);
    // Area 1
    masterStops.emplace_back("Area 1", "1A", "Stop 1");
    masterStops.emplace_back("Area 1", "1B", "Stop 2");
    
    // Area 2
    masterStops.emplace_back("Area 2", "2A", "Stop 3");
    masterStops.emplace_back("Area 2", "2B", "Stop 4");
    
    // Area 3
    masterStops.emplace_back("Area 3", "3A", "Stop 5");
    masterStops.emplace_back("Area 3", "3B", "Stop 6");
}

void initializeRoutes(vector<Route>& allRoutes, vector<Stop>& masterStops) {
    // Route 1: Sequential Loop 1
    Route r1("Route 1 (Short Loop)");
    // Stop 1 -> Stop 2 (5km), Stop 2 -> Stop 3 (5km), Stop 3 -> Stop 1 (10km)
    r1.addStop(findStop(masterStops, "Area 1", "1A", "Stop 1"), 5.0);
    r1.addStop(findStop(masterStops, "Area 1", "1B", "Stop 2"), 5.0);
    r1.addStop(findStop(masterStops, "Area 2", "2A", "Stop 3"), 10.0); 
    allRoutes.push_back(r1);

    // Route 2: Sequential Loop 2
    Route r2("Route 2 (Long Loop)");
    // Stop 3 -> Stop 4 (4km), Stop 4 -> Stop 5 (4km), Stop 5 -> Stop 6 (4km), Stop 6 -> Stop 3 (12km)
    r2.addStop(findStop(masterStops, "Area 2", "2A", "Stop 3"), 4.0);
    r2.addStop(findStop(masterStops, "Area 2", "2B", "Stop 4"), 4.0);
    r2.addStop(findStop(masterStops, "Area 3", "3A", "Stop 5"), 4.0);
    r2.addStop(findStop(masterStops, "Area 3", "3B", "Stop 6"), 12.0);
    allRoutes.push_back(r2);

    // Route 3: Cross-Area Shuttle
    Route r3("Route 3 (Express Loop)");
    // Stop 1 -> Stop 4 (15km), Stop 4 -> Stop 6 (15km), Stop 6 -> Stop 1 (30km)
    r3.addStop(findStop(masterStops, "Area 1", "1A", "Stop 1"), 15.0);
    r3.addStop(findStop(masterStops, "Area 2", "2B", "Stop 4"), 15.0);
    r3.addStop(findStop(masterStops, "Area 3", "3B", "Stop 6"), 30.0);
    allRoutes.push_back(r3);
}

struct BusFleet {
    vector<Bus*> allBuses;

    ~BusFleet() {
        for (Bus* bus : allBuses) {
            delete bus;
        }
    }
};

// Initialize all buses and assign to routes
BusFleet initializeMasterBuses(vector<Route>& allRoutes) {
    BusFleet fleet;

    MiniBus* mini1 = new MiniBus("MIN-101");
    MiniBus* mini2 = new MiniBus("MIN-102");
    MiniBus* mini3 = new MiniBus("MIN-103");
    MiniBus* mini4 = new MiniBus("MIN-104");

    ElectricBus* electric1 = new ElectricBus("ELE-201");
    ElectricBus* electric2 = new ElectricBus("ELE-202");

    DoubleDecker* dd1 = new DoubleDecker("DD-301");
    DoubleDecker* dd2 = new DoubleDecker("DD-302");

    // Assign buses to specific routes

    if (allRoutes.size() > 0) {
        electric1->assignRoute(&allRoutes[0]);
        dd1->assignRoute(&allRoutes[0]);
        mini1->assignRoute(&allRoutes[0]);
    }

    if (allRoutes.size() > 1) {
        mini2->assignRoute(&allRoutes[1]);
        mini3->assignRoute(&allRoutes[1]);
        electric2->assignRoute(&allRoutes[1]);
    }

    if (allRoutes.size() > 2) {
        mini4->assignRoute(&allRoutes[2]);
        dd2->assignRoute(&allRoutes[2]);
    }

    // Add all buses to fleet
    fleet.allBuses.push_back(mini1);
    fleet.allBuses.push_back(mini2);
    fleet.allBuses.push_back(mini3);
    fleet.allBuses.push_back(mini4);
    fleet.allBuses.push_back(electric1);
    fleet.allBuses.push_back(electric2);
    fleet.allBuses.push_back(dd1);
    fleet.allBuses.push_back(dd2);

    return fleet;
}

int getValidatedChoice(string prompt, int maxRange) {
    int choice;
    while (true) {
        cout << prompt;

        if (cin >> choice) {
            if (choice >= 1 && choice <= maxRange) {
                return choice;
            }
            else {
                cout << "Error: " << choice << " is out of range. Choose 1 to " << maxRange << ".\n";
            }
        }
        else {
            cout << "Error: Please enter a numeric value.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

// Get all unique areas from the master list
set<string> getAllUniqueAreas(const vector<Stop>& masterStops) {
    set<string> uniqueAreas;
    for (const auto& s : masterStops) {
        uniqueAreas.insert(s.getArea());
    }
    return uniqueAreas;
}

// Get all stops within a specific area
vector<Stop*> getAllStopsInArea(vector<Stop>& masterStops, const string& targetArea) {
    vector<Stop*> filtered;
    for (auto& stop : masterStops) {
        if (stop.getArea() == targetArea) {
            filtered.push_back(&stop);
        }
    }
    return filtered;
}

// Get unique areas reachable after a specific pickup stop
set<string> getReachableAreasFromStop(const vector<Route>& allRoutes, Stop* pickupStop) {
    set<string> reachableAreas;
    for (const auto& route : allRoutes) {
        const auto& stopsInRoute = route.getStops();
        auto it = find(stopsInRoute.begin(), stopsInRoute.end(), pickupStop);

        if (it != stopsInRoute.end()) {
            size_t pickupIdx = it - stopsInRoute.begin();
            // Forward from pickup+1 to end
            for (size_t i = pickupIdx + 1; i < stopsInRoute.size(); ++i) {
                reachableAreas.insert(stopsInRoute[i]->getArea());
            }
            // Then wrap to pickup-1
            for (size_t i = 0; i < pickupIdx; ++i) {
                reachableAreas.insert(stopsInRoute[i]->getArea());
            }
        }
    }
    return reachableAreas;
}

// Get specific stops in a target area reachable from a pickup stop
set<Stop*> getReachableStopsInArea(const vector<Route>& allRoutes, Stop* pickupStop, const string& targetArea) {
    set<Stop*> uniqueDropoffStops;
    for (const auto& route : allRoutes) {
        const auto& stopsInRoute = route.getStops();
        auto itPickup = find(stopsInRoute.begin(), stopsInRoute.end(), pickupStop);

        if (itPickup != stopsInRoute.end()) {
            size_t pickupIdx = itPickup - stopsInRoute.begin();
            // Forward from pickup+1 to end
            for (size_t i = pickupIdx + 1; i < stopsInRoute.size(); ++i) {
                if (stopsInRoute[i]->getArea() == targetArea) {
                    uniqueDropoffStops.insert(stopsInRoute[i]);
                }
            }
            // Then wrap to pickup-1
            for (size_t i = 0; i < pickupIdx; ++i) {
                if (stopsInRoute[i]->getArea() == targetArea) {
                    uniqueDropoffStops.insert(stopsInRoute[i]);
                }
            }
        }
    }
    return uniqueDropoffStops;
}

vector<Bus*> getValidBusesForTrip(const BusFleet& fleet, Stop* pickup, Stop* dropoff) {
    vector<Bus*> valid;

    for (Bus* bus : fleet.allBuses) {
        Route* r = bus->getAssignedRoute();
        if (!r) continue;

        const auto& stops = r->getStops();
        auto itPickup = find(stops.begin(), stops.end(), pickup);
        auto itDropoff = find(stops.begin(), stops.end(), dropoff);

        if (itPickup != stops.end() && itDropoff != stops.end()) {
            valid.push_back(bus);
        }
    }
    return valid;
}

bool handleTripConfirmation(Bus*& selectedBus, const vector<Bus*>& validBuses, 
                            Stop* pickupStop, Stop* dropoffStop, 
                            double travelDistance, Ticket*& activeTicket) {
    bool confirmed = false;

    while (!confirmed) {
        // Calculate Fare for the current selection
        double finalFare = selectedBus->calculateFare(travelDistance);

        // Display the Detailed Summary
        cout << "============================================================\n";
        cout << "                    FINAL TRIP SUMMARY\n";
        cout << "============================================================\n";
        cout << "PICKUP:    " << pickupStop->getFullName() << "\n";
        cout << "DROPOFF:   " << dropoffStop->getFullName() << "\n";
        cout << "DISTANCE:  " << travelDistance << " km\n";
        cout << "------------------------------------------------------------\n";
        cout << "BUS DETAILS:\n";
        cout << "   Number:    " << selectedBus->getBusNumber() << "\n";
        cout << "   Type:      " << selectedBus->getBusType() << "\n";
        cout << "   Status:    " << (selectedBus->isOvercrowded() ? "Standing Room Only" : "Seats Available") << "\n";
        cout << "------------------------------------------------------------\n";
        cout << "FARE DETAILS:\n";
        cout << "   Rate:      Rs " << selectedBus->getFareRate() << "/km\n";
        cout << "   Min Fare:  Rs " << selectedBus->getMinimumFare() << "\n";
        cout << "   Max Fare:  Rs " << selectedBus->getMaximumFare() << "\n";
        cout << "   TOTAL:     Rs " << finalFare << "\n";
        cout << "============================================================\n";

        // 3 Options
        cout << "\nWHAT WOULD YOU LIKE TO DO?\n";
        cout << "[1] Finalize Payment\n";
        cout << "[2] Change Bus (View List Again)\n";
        cout << "[3] Cancel Everything\n\n";

        int postSummaryChoice = getValidatedChoice("Enter choice: ", 3);

        if (postSummaryChoice == 1) {
            if (selectedBus->canBoardPassenger()) {
                activeTicket = new Ticket(selectedBus, pickupStop, dropoffStop);
                
                cout << "\n[SUCCESS] Payment processed! Ticket registered.\n\n";
                cout << "Your bus (" << selectedBus->getBusNumber() << ") is on its way.\n";
                cout << "Please wait at stop \"" << pickupStop->getFullName() << "\".\n";

                confirmed = true; 
            } else {
                cout << "\n[FAILED] Sorry, this bus reached its limit while you were viewing the summary.\n";
                cout << "Please select another bus.\n\n";
                postSummaryChoice = 2; // Transition logic: manually set choice to 2 to trigger re-selection
            }
        }

        if (postSummaryChoice == 2) {
            cout << "\nRE-SELECTING BUS:\n\n";
            for (size_t i = 0; i < validBuses.size(); ++i) {
                cout << "[" << i + 1 << "] " << validBuses[i]->getBusNumber()
                    << " (" << validBuses[i]->getBusType() << ")\n";
            }
            int newChoice = getValidatedChoice("\nSelect your bus (Number): ", (int)validBuses.size());
            selectedBus = validBuses[newChoice - 1];
            cout << "\n------------------------------------------------------------\n\n";
            // Loop naturally continues to show the new summary
        } 
        else if (postSummaryChoice == 3) {
            cout << "\n[CANCELLED] Trip aborted. Thank you for using Smart Transport!\n";
            return false; // Tells main to exit the program
        }
    }
    return true; // Ticket created successfully
}

void runSimulation(BusFleet& fleet, Ticket* activeTicket, Bus* selectedBus, Stop* pickupStop, Stop* dropoffStop) {
    string command;
    bool journeyActive = true;

    cout << ">>> SIMULATION STARTED: Waiting for your bus at \"" << pickupStop->getFullName() << "\"\n";
    cout << "Type '/tick' to advance time.\n";

    while (journeyActive) {
        cout << "\n[PROMPT]: ";
        cin >> command;

        if (command == "/tick") {
            // Move the entire fleet
            for (Bus* b : fleet.allBuses) {
                b->move();
            }

            // Check Ticket State
            if (activeTicket->getState() == JourneyState::WAITING_FOR_PICKUP) {
                cout << "\nYour bus (" << selectedBus->getBusNumber() << ") is currently at: "
                    << selectedBus->getCurrentStop()->getFullName() << "\n";

                if (activeTicket->isAtPickup()) {
                    cout << "\nNOTIFICATION: Your bus has arrived at \"" << pickupStop->getFullName() << "\"!\n\n";

                    cout << "Options:\n";
                    cout << "[1] Board Bus\n";
                    cout << "[2] Cancel Journey\n\n";

                    int boardChoice = getValidatedChoice("Choice: ", 2);
                    if (boardChoice == 1) {
                        if (selectedBus->boardPassenger()) {
                            activeTicket->setBoarded();
                            cout << "\nYou have boarded the bus!\n";
                        }
                        else {
                            cout << "\nBus is full!\n";
                        }
                    }
                    else {
                        cout << "\nJourney cancelled.\n";
                        journeyActive = false;
                    }
                }
            }
            else if (activeTicket->getState() == JourneyState::ON_BOARD) {
                cout << "\nYou are on board.\nBus reached stop: " << selectedBus->getCurrentStop()->getFullName() << "\n";

                if (activeTicket->isAtDestination()) {
                    activeTicket->setCompleted();
                    cout << "\nSUCCESS: You have reached your destination (" << dropoffStop->getFullName() << ")!\n\n";
                    cout << "Thank you for using Smart Transport.\n";
                    journeyActive = false;
                }
            }
        }
        else {
            cout << "Unknown command. Use /tick to move buses.\n";
        }
    }
}

int main() {
    cout << "============================================================\n";
    cout << "              WELCOME TO SMART TRANSPORT\n";
    cout << "============================================================\n";

    // Data containers initialized in main
    vector<Stop> masterStops;
    vector<Route> allRoutes;

    // Call separate functions to populate data
    initializeMasterStops(masterStops);
    initializeRoutes(allRoutes, masterStops);

    // Initialize bus fleet
    BusFleet fleet = initializeMasterBuses(allRoutes);

    cout << "AVAILABLE AREAS:\n";

    set<string> uniqueAreas = getAllUniqueAreas(masterStops);

    for (int displayIndex = 0; const auto& area : uniqueAreas) {
        cout << "[" << ++displayIndex << "] " << area << '\n';
    }

    int pickupAreaChoice = getValidatedChoice("\nEnter your pickup area (Number): ", uniqueAreas.size());
    string pickupArea = *next(uniqueAreas.begin(), pickupAreaChoice - 1);

    cout << "\nSelected Pickup Area: " << pickupArea << "\n";

    cout << "\n------------------------------------------------------------\n\n";

    cout << "STOPS IN " << pickupArea << ":\n";

    vector<Stop*> filteredPickupStops = getAllStopsInArea(masterStops, pickupArea);

    for (size_t i = 0; i < filteredPickupStops.size(); ++i) {
        cout << "[" << i + 1 << "] " << filteredPickupStops[i]->getFullName() << '\n';
    }

    int pickupStopChoice = getValidatedChoice("\nEnter your pickup stop (Number): ", filteredPickupStops.size());;
    Stop* pickupStop = filteredPickupStops[pickupStopChoice - 1];

    cout << "Pickup Stop confirmed: " << pickupStop->getFullName() << "\n";

    cout << "\n------------------------------------------------------------\n\n";

    set<string> reachableAreas = getReachableAreasFromStop(allRoutes, pickupStop);

    cout << "DESTINATION AREAS REACHABLE FROM STOP \"" << pickupStop->getFullName() << "\":\n";

    if (reachableAreas.empty()) {
        cout << "No destinations reachable from this stop.\n";
        return 0;
    }

    for (int i = 0; const auto& area : reachableAreas) {
        cout << "[" << ++i << "] " << area << '\n';
    }

    int dropoffAreaChoice = getValidatedChoice("\nEnter your dropoff area (Number): ", reachableAreas.size());
    string dropoffArea = *next(reachableAreas.begin(), dropoffAreaChoice - 1);

    cout << "\nSelected Dropoff Area: " << dropoffArea << "\n";

    cout << "\n------------------------------------------------------------\n\n";

    set<Stop*> uniqueDropoffStops = getReachableStopsInArea(allRoutes, pickupStop, dropoffArea);

    // 2. Display the filtered drop-off stops
    cout << "STOPS IN " << dropoffArea << " (REACHABLE FROM \"" << pickupStop->getFullName() << "\"):\n";
    for (int i = 0; const auto& dropoffStop : uniqueDropoffStops) {
        cout << "[" << ++i << "] " << dropoffStop->getFullName() << "\n";
    }

    // 3. Get User Choice
    int dropoffStopChoice = getValidatedChoice("\nEnter your drop-off stop (Number): ", uniqueDropoffStops.size());
    Stop* dropoffStop = *next(uniqueDropoffStops.begin(), dropoffStopChoice - 1);

    cout << "Drop-off Stop confirmed: " << dropoffStop->getFullName() << "\n";

    cout << "\n------------------------------------------------------------\n\n";

    cout << "AVAILABLE BUSES FOR THIS ROUTE:\n";

    vector<Bus*> validBuses = getValidBusesForTrip(fleet, pickupStop, dropoffStop);

    if (validBuses.empty()) {
        cout << "No buses currently serving this specific connection.\n";
        return 0;
    }

    for (size_t i = 0; i < validBuses.size(); ++i) {
        cout << "[" << i + 1 << "] " << validBuses[i]->getBusNumber() 
            << " (" << validBuses[i]->getBusType() << ")\n";
    }

    int busChoice = getValidatedChoice("\nSelect your bus (Number): ", validBuses.size());
    Bus* selectedBus = validBuses[busChoice - 1];

    cout << "\n------------------------------------------------------------\n\n";

    double travelDistance = selectedBus->getAssignedRoute()->getDistanceBetween(pickupStop, dropoffStop);
    Ticket* activeTicket = nullptr;

    if (!handleTripConfirmation(selectedBus, validBuses, pickupStop, dropoffStop, travelDistance, activeTicket)) {
        return 0; // Exit if trip was cancelled
    }

    cout << "\n------------------------------------------------------------\n\n";

    runSimulation(fleet, activeTicket, selectedBus, pickupStop, dropoffStop);

    if (activeTicket != nullptr) {
        delete activeTicket;
    }

    return 0;
}