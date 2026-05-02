#ifndef STOP_H
#define STOP_H

#include <string>
#include <vector>

using namespace std;

class Stop {
private:
    string area;
    string block;
    string landmark;
    float distance;

public:
    Stop(string a, string b, string l, float d);

    friend Stop* findStop(vector<Stop>& masterStops, const string& area, const string& block, const string& landmark);
    
    string getArea() const;
    string getBlock() const;
    string getLandmark() const;
    float getDistance() const;
    string getFullName() const;
};

Stop* findStop(vector<Stop>& masterStops, const string& area, const string& block, const string& landmark);

#endif