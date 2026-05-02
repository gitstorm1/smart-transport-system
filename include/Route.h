#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <vector>
#include "Stop.h"

class Route {
private:
    std::string routeName;
    std::vector<Stop*> stops;

public:
    Route(std::string name);

    void addStop(Stop* s);

    std::vector<Stop*> getStops() const;

    // Displays the whole route (i.e. all stops)
    void displayRoute() const;
};

#endif