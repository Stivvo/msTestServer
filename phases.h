#ifndef PHASES_H
#define PHASES_H

#include <iostream>
#include <vector>
#include <utility>
#include <QString>
#include <fstream>
#include <sstream>
#include <string>
#include <QDebug>

class Phases
{
public:
    Phases();
    ~Phases() = default;
    bool add(std::string name, bool enabled, int number); // return false if name doesn't exists
    bool parseLine(std::string line);

    bool advance();
    int getCurrent() {return current;}
    int size() {return names.size();}
    bool isLast();
    bool finished();
    bool isFirst();

    QString currentName();
    bool currentEnabled();
    int currentNumber();
    bool is_number(const std::string& s);
private:
    std::vector<QString> names;
    std::vector<std::pair<bool, int>> values;
    int current;
};

#endif // PHASES_H
