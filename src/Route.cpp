#include "../include/Route.h"
#include <iostream>

using namespace std;

Route::Route(string name) : routeName(name) {}

void Route::addStop(Stop* s, double distance) {
    if (s != nullptr) {
        stops.push_back(s);
        segmentDistances.push_back(distance);
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
    for (size_t i = 0; i < stops.size(); i++) {
        cout << i << ". " << stops[i]->getFullName();
        if (i < segmentDistances.size()) {
            cout << " [" << segmentDistances[i] << " km to next]";
        }
        cout << endl;
    }
}

double Route::getDistanceBetween(Stop* start, Stop* end) const {
    if (start == end) return 0.0;

    auto itStart = find(stops.begin(), stops.end(), start);
    auto itEnd = find(stops.begin(), stops.end(), end);

    if (itStart == stops.end() || itEnd == stops.end()) return -1.0;

    size_t startIdx = itStart - stops.begin();
    size_t endIdx = itEnd - stops.begin();

    if (startIdx < endIdx) {
        double dist = 0.0;
        for (size_t i = startIdx; i < endIdx; ++i) {
            dist += segmentDistances[i];
        }
        return dist;
    } else {
        double dist = 0.0;
        size_t stopsSize = stops.size();
        
        for (size_t i = startIdx; i < stopsSize; ++i) {
            dist += segmentDistances[i];
        }
        
        for (size_t i = 0; i < endIdx; ++i) {
            dist += segmentDistances[i];
        }
        
        return dist;
    }
}