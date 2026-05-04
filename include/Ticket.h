#ifndef TICKET_H
#define TICKET_H

#include "Bus.h"
#include "Stop.h"

enum class JourneyState { WAITING_FOR_PICKUP, ON_BOARD, COMPLETED };

class Ticket {
private:
    Bus* bookedBus;
    Stop* pickup;
    Stop* dropoff;
    JourneyState currentState;

public:
    Ticket(Bus* b, Stop* p, Stop* d) 
        : bookedBus(b), pickup(p), dropoff(d), currentState(JourneyState::WAITING_FOR_PICKUP) {}

    Bus* getBus() const { return bookedBus; }
    JourneyState getState() const { return currentState; }
    void setBoarded() { currentState = JourneyState::ON_BOARD; }
    void setCompleted() { currentState = JourneyState::COMPLETED; }

    bool isAtPickup() const {
        return bookedBus->getCurrentStop() == pickup;
    }

    bool isAtDestination() const {
        return bookedBus->getCurrentStop() == dropoff;
    }
};

#endif