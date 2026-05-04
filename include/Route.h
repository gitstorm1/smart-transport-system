#ifndef ROUTE_H
#define ROUTE_H

#include <string>
#include <vector>
#include "Stop.h"

class Route {
private:
    std::string routeName;
    // Aggregation: Route keeps references to existing Stop objects
    std::vector<Stop*> stops;
    std::vector<double> segmentDistances;

public:
    Route(std::string name);

    // Encapsulation: Route operations are exposed through member functions
    void addStop(Stop* s, double distance);

    std::vector<Stop*> getStops() const;

    double getDistanceBetween(Stop* start, Stop* end) const;

    void displayRoute() const;
};

#endif