#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <QDebug>
#include <QString>

class Phases
{
public:
    Phases();
    ~Phases() = default;
    bool add(std::string name, bool enabled, int number); // return false if name doesn't exists
    bool parseLine(std::string line);

    bool advance();
    int getCurrent() { return current; }
    int size() { return names.size(); }
    bool isLast();
    bool finished();
    bool isFirst();

    QString currentName();
    bool currentEnabled();
    int currentNumber();
    bool is_number(const std::string &s);
    void print();

private:
    std::vector<QString> names;
    std::vector<std::pair<bool, int>> values;
    int current;
};
