#ifndef STOP_H
#define STOP_H

#include <string>
#include <vector>

class Stop {
private:
    std::string area;
    std::string block;
    std::string landmark;

public:
    Stop(std::string a, std::string b, std::string l);

    friend Stop* findStop(std::vector<Stop>& masterStops, const std::string& area, const std::string& block, const std::string& landmark);
    
    std::string getArea() const;
    std::string getBlock() const;
    std::string getLandmark() const;
    std::string getFullName() const;
};

Stop* findStop(std::vector<Stop>& masterStops, const std::string& area, const std::string& block, const std::string& landmark);

#endif