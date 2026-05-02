#include "../include/Stop.h"
#include <string>

using namespace std;

Stop::Stop(string a, string b, string l, float d) 
    : area(a), block(b), landmark(l), distance(d) {}

string Stop::getArea() const {
    return area;
}

string Stop::getBlock() const {
    return block;
}

string Stop::getLandmark() const {
    return landmark;
}

float Stop::getDistance() const { 
    return distance; 
}

string Stop::getFullName() const {
    string name = area + " (Block " + block + ")";
    if (!landmark.empty()) {
        name += " - near " + landmark;
    }
    return name;
}

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