#ifndef PHASES_H
#define PHASES_H

#include <iostream>
#include <vector>
#include <vector>
#include <utility>
#include <QString>

class Phases
{
public:
    Phases();
    ~Phases() = default;
    bool add(std::string name, bool enabled, int number); // return false if name doesn't exists

    bool advance();
    int getCurrent() {return current;}
    int size() {return names.size();}
    bool isLast();
    bool finished();
    bool isFirst();

    QString currentName();
    bool currentEnabled();
    int currentNumber();
private:
    std::vector<QString> names;
    std::vector<std::pair<bool, int>> values;
    int current;
};

#endif // PHASES_H
