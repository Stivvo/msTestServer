#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <QDebug>
#include <QSerialPort>
#include <QString>

class Phases
{
public:
    Phases();
    ~Phases() = default;

    bool add(std::string name, bool enabled, int number); // return false if name doesn't exists
    bool parseLine(std::string line);

    bool advance();
    int size() { return names.size(); }
    void print();

    bool isLast();
    bool finished();
    bool isFirst();

    int currentNumber();
    bool currentEnabled();
    QString currentName();

private:
    static const std::vector<QString> names;
    std::vector<std::pair<bool, int>> values;
    int current;

    QString portName;
    QString ip;
    int baudrate;

    bool validBaudrate(std::string s);
    bool validIp(std::string s);
    bool is_number(const std::string &s);
};
