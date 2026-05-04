#include "../include/Stop.h"
#include <string>

using namespace std;

Stop::Stop(string a, string b, string l) 
    : area(a), block(b), landmark(l) {}

string Stop::getArea() const {
    return area;
}

string Stop::getBlock() const {
    return block;
}

string Stop::getLandmark() const {
    return landmark;
}

string Stop::getFullName() const {
    string name = area + " (Block " + block + ")";
    if (!landmark.empty()) {
        name += " | Landmark: " + landmark;
    }
    return name;
}

// Friend function implementation can read private Stop members directly
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