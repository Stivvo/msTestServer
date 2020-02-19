#include "phases.h"

const std::vector<QString> Phases::names = {"start",
                                            "touch",
                                            "brightness",
                                            "turnoffScreen",
                                            "usb",
                                            "red",
                                            "green",
                                            "blue",
                                            "black",
                                            "white",
                                            "end"};

Phases::Phases()
{
    values.reserve(names.size());
    for (unsigned long i = 0; i < names.size(); ++i)
        values.emplace_back(std::make_pair(false, 0));
    current = -1; // phase -1 is "press a button to start"
}

bool Phases::advance()
{
    if (current < static_cast<int>(names.size()) || current == -1) {
        ++current;
        return true;
    }
    return false;
}

bool Phases::isLast()
{
    return current == this->size() - 1;
}

bool Phases::finished()
{
    return current == this->size();
}

bool Phases::isFirst()
{
    return current == -1;
}

void Phases::print()
{
    for (unsigned long i = 0; i < names.size(); ++i)
        qDebug() << names.at(i) << ", " << values.at(i).first << ", " << values.at(i).second;
}

bool Phases::add(std::string nameStd, bool enabled, int number)
{
    bool ret = false;
    unsigned long i = 0;
    QString name = QString::fromStdString(
        nameStd); // costava tanto a qt fare un costruttore con parametri per qstring che prendeva std::string
    while (i < names.size() && !ret) {
        if (names.at(i) == name) {
            qDebug() << "adding: " << QString::fromStdString(nameStd) << ", enabled: " << enabled
                     << ", number: " << number;
            ret = true;
            values.at(i) = std::make_pair(enabled, number);
            qDebug() << "found in " << i;
        }
        ++i;
    }
    return ret;
}

bool Phases::parseLine(std::string line)
{
    std::istringstream buffer(line);

    std::string temp;
    std::string name;
    bool enabled = false;
    int number = 0;

    while (std::getline(buffer, temp, ';')) {
        qDebug() << "column: " << QString::fromStdString(temp);
        if (temp == "on")
            enabled = true;
        else if (temp == "off")
            enabled = false;
        else if (is_number(temp))
            number = std::stoi(temp);
        else
            name = temp;
    }
    return this->add(name, enabled, number);
}

bool Phases::is_number(const std::string &s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) {
                             return !std::isdigit(c);
                         }) == s.end();
}

QString Phases::currentName()
{
    return names.at(current);
}

bool Phases::currentEnabled()
{
    if (this->finished())
        return true;
    return values.at(current).first;
}

int Phases::currentNumber()
{
    return values.at(current).second;
}
