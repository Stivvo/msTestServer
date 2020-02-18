#ifndef PHASES_H
#define PHASES_H

#include <iostream>
#include <vector>
#include <vector>
#include <utility>

class Phases
{
public:
    Phases();
    ~Phases() = default;
    bool add(std::string name, bool enabled, int number); // return false if name doesn't exists
    void advance();
    int getCurrentPhase() {return current;}
    int nrPhases() {return names.size();}
    std::string currentName();
    bool currentEnabled();
    int currentNumber();
private:
    std::vector<std::string> names;
    std::vector<std::pair<bool, int>> values;
    int current;
};

#endif // PHASES_H
