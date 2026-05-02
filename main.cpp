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
    int maxOvercrowding;  // Maximum passengers, including those standing (overcrowding)

public:
    Bus(string busNum, string busTyp, int cap, double fareRat, double minFare, double maxFare) 
        :
            busNumber(busNum),
            busType(busTyp),
            capacity(cap), maxOvercrowding(cap), currentPassengers(0),
            fareRate(fareRat), minimumFare(minFare), maximumFare(maxFare),
            assignedRoute(nullptr) {}
    
    virtual ~Bus() {}
    
    virtual double calculateFare(double distance) const = 0;

    virtual bool boardPassenger() {
        int effectiveCapacity = maxOvercrowding;
        
        if (currentPassengers < effectiveCapacity) {
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
        } else {
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

void initializeMasterStops(vector<Stop>& masterStops) {
    masterStops.reserve(100); 
    masterStops.emplace_back("North Sector", "A", "Gate 1", 0.0);
    masterStops.emplace_back("Downtown", "C", "Central Library", 5.2);
    masterStops.emplace_back("Industrial Zone", "B", "Steel Mill", 12.5);
    masterStops.emplace_back("Downtown", "F", "City Hall", 2.1);
    masterStops.emplace_back("Residential East", "D", "Public Park", 6.8);
}

void initializeRoutes(vector<Route>& allRoutes, vector<Stop>& masterStops) {
    // Route 1: North-South Express
    Route r1("North-South Express");
    r1.addStop(findStop(masterStops, "North Sector", "A", "Gate 1"));
    r1.addStop(findStop(masterStops, "Downtown", "C", "Central Library"));
    r1.addStop(findStop(masterStops, "Industrial Zone", "B", "Steel Mill"));
    allRoutes.push_back(r1);

    // Route 2: Downtown Circular
    Route r2("Downtown Loop");
    r2.addStop(findStop(masterStops, "Downtown", "C", "Central Library"));
    r2.addStop(findStop(masterStops, "Downtown", "F", "City Hall"));
    r2.addStop(findStop(masterStops, "Residential East", "D", "Public Park"));
    allRoutes.push_back(r2);

    // Route 3: Worker's Shuttle
    Route r3("Industrial Shuttle");
    r3.addStop(findStop(masterStops, "Residential East", "D", "Public Park"));
    r3.addStop(findStop(masterStops, "Downtown", "F", "City Hall"));
    r3.addStop(findStop(masterStops, "Industrial Zone", "B", "Steel Mill"));
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
            } else {
                cout << "Error: " << choice << " is out of range. Choose 1 to " << maxRange << ".\n";
            }
        } else {
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
            for (auto nextStop = it + 1; nextStop != stopsInRoute.end(); ++nextStop) {
                reachableAreas.insert((*nextStop)->getArea());
            }
        }
    }
    // MAYBE: Have it respect route order
    return reachableAreas;
}

// Get specific stops in a target area reachable from a pickup stop
set<Stop*> getReachableStopsInArea(const vector<Route>& allRoutes, Stop* pickupStop, const string& targetArea) {
    set<Stop*> uniqueDropoffStops;
    for (const auto& route : allRoutes) {
        const auto& stopsInRoute = route.getStops();
        
        auto itPickup = find(stopsInRoute.begin(), stopsInRoute.end(), pickupStop);

        if (itPickup != stopsInRoute.end()) {
            // Look at every stop AFTER the pickup stop
            for (auto itDest = itPickup + 1; itDest != stopsInRoute.end(); ++itDest) {
                Stop* potentialStop = *itDest;
                
                if (potentialStop->getArea() == targetArea) {
                    uniqueDropoffStops.insert(potentialStop);
                }
            }
        }
    }
    return uniqueDropoffStops;
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

    for (int i = 0; i < filteredPickupStops.size(); ++i) {
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
    cout << "STOPS IN " << dropoffArea << " (REACHABLE FROM " << pickupStop->getArea() << "):\n";
    for (int i = 0; const auto& dropoffStop : uniqueDropoffStops) {
        cout << "[" << ++i << "] " << dropoffStop->getFullName() << "\n";
    }

    // 3. Get User Choice
    int dropoffStopChoice = getValidatedChoice("\nEnter your drop-off stop (Number): ", uniqueDropoffStops.size());
    Stop* dropoffStop = *next(uniqueDropoffStops.begin(), dropoffStopChoice - 1);

    cout << "Drop-off Stop confirmed: " << dropoffStop->getFullName() << "\n";

    cout << "\n------------------------------------------------------------\n\n";

    cout << "AVAILABLE BUSES FOR THIS ROUTE:\n";

    vector<Bus*> validBuses;
    double travelDistance = abs(dropoffStop->getDistance() - pickupStop->getDistance());

    for (Bus* bus : fleet.allBuses) {
        Route* r = bus->getAssignedRoute();
        if (!r) continue; // Skip buses with no route assigned

        const auto& stops = r->getStops();
        auto itPickup = find(stops.begin(), stops.end(), pickupStop);
        auto itDropoff = find(stops.begin(), stops.end(), dropoffStop);

        // Check if both stops exist on this bus's route AND pickup is before dropoff
        if (itPickup != stops.end() && itDropoff != stops.end() && itPickup < itDropoff) {
            validBuses.push_back(bus);
            
            // Display bus info and the calculated fare for this specific bus type
            cout << "[" << validBuses.size() << "] " 
                 << bus->getBusNumber() << " (" << bus->getBusType() << ") | "
                 << "Fare: Rs " << bus->calculateFare(travelDistance) << "\n";
        }
    }

    if (validBuses.empty()) {
        cout << "No buses currently serving this specific connection.\n";
        return 0;
    }

    int busChoice = getValidatedChoice("\nSelect your bus (Number): ", validBuses.size());
    Bus* selectedBus = validBuses[busChoice - 1];

    cout << "\n------------------------------------------------------------\n\n";

    double finalFare = selectedBus->calculateFare(travelDistance);

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
    cout << "============================================================\n\n";

    

    cout << "[SUCCESS] Ticket registered!" << '\n';

    cout << R"(============================================================
LIVE NOTIFICATIONS
============================================================
)";

    

    return 0;
}