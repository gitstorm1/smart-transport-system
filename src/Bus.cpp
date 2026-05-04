#include "../include/Bus.h"
#include <algorithm>

using namespace std;

// Encapsulation: constructor initializes internal bus state
Bus::Bus(string busNum, string busTyp, int cap, double fareRat, double minFare, double maxFare)
    :
    busNumber(busNum),
    busType(busTyp),
    capacity(cap), maxOvercrowding(cap), currentPassengers(0), currentStopIndex(0),
    fareRate(fareRat), minimumFare(minFare), maximumFare(maxFare),
    assignedRoute(nullptr) {
}

// Encapsulated behavior: Bus manages its own movement and boarding logic
void Bus::move() {
    if (assignedRoute == nullptr) return;
    const auto& stops = assignedRoute->getStops();
    currentStopIndex = (currentStopIndex + 1) % stops.size();
}

Stop* Bus::getCurrentStop() const {
    if (!assignedRoute) return nullptr;
    return assignedRoute->getStops()[currentStopIndex];
}

bool Bus::canBoardPassenger() const {
    return currentPassengers < maxOvercrowding;
}

bool Bus::boardPassenger() {
    if (canBoardPassenger()) {
        ++currentPassengers;
        return true;
    }
    return false;
}

void Bus::displayBasicInfo() const {
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

void Bus::assignRoute(Route* route) { assignedRoute = route; }

bool Bus::setOvercrowdingLimit(int maxLimit) {
    if (maxLimit < capacity) return false;
    maxOvercrowding = maxLimit;
    return true;
}

bool Bus::isOvercrowded() const { return currentPassengers > capacity; }

// Derived Implementation
MiniBus::MiniBus(string busNum)
    : Bus(busNum, "Minibus", 26, 3, 50, 100) {
    setOvercrowdingLimit(26 + 45);
}
// Runtime polymorphism: each bus type computes fare differently via override
double MiniBus::calculateFare(double distance) const {
    double fare = min(maximumFare, max(minimumFare, distance * fareRate));
    return fare;
}

ElectricBus::ElectricBus(string busNum)
    : Bus(busNum, "Electric bus", 32, 5, 80, 120) {
    setOvercrowdingLimit(40 + 40);
}
double ElectricBus::calculateFare(double distance) const {
    double fare = min(maximumFare, max(minimumFare, distance * fareRate));
    return fare;
}

DoubleDecker::DoubleDecker(string busNum)
    : Bus(busNum, "Double-Decker bus", 80, 7, 100, 150) {
    setOvercrowdingLimit(80 + 40);
}
double DoubleDecker::calculateFare(double distance) const {
    double fare = min(maximumFare, max(minimumFare, distance * fareRate));
    return fare;
}