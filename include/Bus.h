#ifndef BUS_H
#define BUS_H

#include <string>
#include <vector>
#include <iostream>
#include "Route.h"
#include "Stop.h"

// Encapsulation: bus state is hidden in private/protected members
class Bus {
private:
    int currentPassengers;
protected:
    std::string busNumber;
    std::string busType;
    int capacity;
    double fareRate;
    double minimumFare;
    double maximumFare;
    // Aggregation: Bus holds a non-owning reference to a Route
    Route* assignedRoute;
    int currentStopIndex;
    int maxOvercrowding;

public:
    Bus(std::string busNum, std::string busTyp, int cap, double fareRat, double minFare, double maxFare);
    virtual ~Bus() {}

    // Abstraction / Polymorphism: base interface for fare calculation
    virtual double calculateFare(double distance) const = 0;

    virtual void move();
    virtual Stop* getCurrentStop() const;
    virtual bool canBoardPassenger() const;
    virtual bool boardPassenger();
    virtual void displayBasicInfo() const;
    virtual void assignRoute(Route* route);
    virtual bool setOvercrowdingLimit(int maxLimit);
    virtual bool isOvercrowded() const;

    // Getters
    std::string getBusNumber() const { return busNumber; }
    std::string getBusType() const { return busType; }
    int getCapacity() const { return capacity; }
    int getMaxOvercrowding() const { return maxOvercrowding; }
    int getCurrentPassengers() const { return currentPassengers; }
    double getFareRate() const { return fareRate; }
    double getMinimumFare() const { return minimumFare; }
    double getMaximumFare() const { return maximumFare; }
    Route* getAssignedRoute() const { return assignedRoute; }
};

// Derived Classes
// Inheritance: specialized bus types derive from Bus
class MiniBus : public Bus {
public:
    MiniBus(std::string busNum);
    double calculateFare(double distance) const override;
};

class ElectricBus : public Bus {
public:
    ElectricBus(std::string busNum);
    double calculateFare(double distance) const override;
};

class DoubleDecker : public Bus {
public:
    DoubleDecker(std::string busNum);
    double calculateFare(double distance) const override;
};

#endif