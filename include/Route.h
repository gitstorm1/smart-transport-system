#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <vector>
#include "Stop.h"

class Route {
private:
    std::string routeName;
    std::vector<Stop*> stops;
    std::vector<double> segmentDistances;

public:
    Route(std::string name);

    void addStop(Stop* s, double distance);

    std::vector<Stop*> getStops() const;

    double getDistanceBetween(Stop* start, Stop* end) const;

    void displayRoute() const;
};

#endif