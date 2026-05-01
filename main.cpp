#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <iterator>

using namespace std;

class Stop {
private:
    string area;
    string block;
    string landmark;
    float distance;

public:
    Stop(string a, string b, string l, float d) 
        : area(a), block(b), landmark(l), distance(d) {}

    friend Stop* findStop(vector<Stop>& masterStops, const string& area, const string& block, const string& landmark);
    
    string getArea() const {
        return area;
    }

    string getBlock() const {
        return block;
    }

    string getLandmark() const {
        return landmark;
    }

    float getDistance() const { 
        return distance; 
    }
    
    string getFullName() const {
        string name = area + " (Block " + block + ")";
        if (!landmark.empty()) {
            name += " - near " + landmark;
        }
        return name;
    }
};

class Route {
private:
    string routeName;
    vector<Stop*> stops;

public:
    Route(string name) : routeName(name) {}

    void addStop(Stop* s) {
        if (s != nullptr) {
            stops.push_back(s);
        } else {
            cout << "Error: Attempted to add a null stop to route: " << routeName << '\n';
        }
    }

    vector<Stop*> getStops() const {
        return stops;
    }

    // Displays the whole route (i.e. all stops)
    void displayRoute() const {
        cout << "\n--- Route: " << routeName << " ---" << endl;
        for (int i = 0; i < stops.size(); i++) {
            cout << i << ". " << stops[i]->getFullName() << " ["
                << stops[i]->getDistance() << " km]" << endl;
        }
    }
};

Stop* findStop(vector<Stop>& masterStops, const string& area, const string& block, const string& landmark) {
    for (auto& s : masterStops) {
        if (s.area == area && 
            s.block == block && 
            s.landmark == landmark) {
            return &s;
        }
    }

    return nullptr;
}

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

    cout << "AVAILABLE AREAS:\n";

    set<string> uniqueAreas;

    for (const auto& s : masterStops) {
        uniqueAreas.insert(s.getArea());
    }

    for (int displayIndex = 0; const auto& area : uniqueAreas) {
        cout << "[" << ++displayIndex << "] " << area << '\n';
    }

    int pickupAreaChoice = getValidatedChoice("\nEnter your pickup area (Number): ", uniqueAreas.size());
    string pickupArea = *next(uniqueAreas.begin(), pickupAreaChoice - 1);

    cout << "\nSelected Pickup Area: " << pickupArea << "\n";

    cout << "\n------------------------------------------------------------\n\n";

    cout << "STOPS IN " << pickupArea << ":\n";

    vector<Stop*> filteredStops;

    for (int displayIndex = 0; auto& stop : masterStops) {
        if (stop.getArea() != pickupArea) continue;
        filteredStops.push_back(&stop);
        cout << "[" << ++displayIndex << "] " << stop.getFullName() << '\n';
    }

    int pickupStopChoice = getValidatedChoice("\nEnter your pickup stop (Number): ", filteredStops.size());;
    Stop* pickupStop = filteredStops[pickupStopChoice - 1];

    cout << "Pickup Stop confirmed: " << pickupStop->getFullName() << "\n";

    cout << "\n------------------------------------------------------------\n\n";

    set<string> reachableAreas;

    for (const auto& route : allRoutes) {
        const auto& stopsInRoute = route.getStops();
        
        // Find the pickupStop in this route
        auto it = find(stopsInRoute.begin(), stopsInRoute.end(), pickupStop);

        if (it != stopsInRoute.end()) {
            // If found, every stop AFTER this one is a valid destination
            // we use it + 1 to skip the pickup stop itself
            for (auto nextStop = it + 1; nextStop != stopsInRoute.end(); ++nextStop) {
                reachableAreas.insert((*nextStop)->getArea());
            }
        }
    }

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

    cout << "STOPS IN <DROPOFF AREA NAME> (ON CONNECTED ROUTES):" << '\n';

    cout << "Enter your dropoff stop (Number):" << '\n';

    cout << "------------------------------------------------------------" << '\n';

    cout << "AVAILABLE BUSES FOR THIS ROUTE:" << '\n';

    cout << "Select your bus (Number):" << '\n';

    cout << "------------------------------------------------------------" << '\n';

    cout << "TRIP SUMMARY:" << '\n';

    cout << R"(Route:    Central Station -> Tech Park
Distance: 8.4 units
Fare:     $12.50

Finalize payment? (y/n): y
)";

    cout << "[SUCCESS] Ticket registered!" << '\n';

    cout << R"(============================================================
LIVE NOTIFICATIONS
============================================================
)";

    

    return 0;
}