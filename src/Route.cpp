#include "../include/Route.h"
#include <iostream>

using namespace std;

Route::Route(string name) : routeName(name) {}

void Route::addStop(Stop* s) {
    if (s != nullptr) {
        stops.push_back(s);
    } else {
        cout << "Error: Attempted to add a null stop to route: " << routeName << '\n';
    }
}

vector<Stop*> Route::getStops() const {
    return stops;
}

// Displays the whole route (i.e. all stops)
void Route::displayRoute() const {
    cout << "\n--- Route: " << routeName << " ---" << endl;
    for (int i = 0; i < stops.size(); i++) {
        cout << i << ". " << stops[i]->getFullName() << " ["
            << stops[i]->getDistance() << " km]" << endl;
    }
}