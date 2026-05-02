#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <vector>
#include "Stop.h"

using namespace std;

class Route {
private:
    string routeName;
    vector<Stop*> stops;

public:
    Route(string name);

    void addStop(Stop* s);

    vector<Stop*> getStops() const;

    // Displays the whole route (i.e. all stops)
    void displayRoute() const;
};

#endif