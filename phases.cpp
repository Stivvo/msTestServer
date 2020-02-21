#include "phases.h"

const std::vector<QString> Phases::names = {
    "start",
    "touch",
    "brightness",
    "turnoffScreen",
    "usb",
    "red",
    "green",
    "blue",
    "black",
    "white",
    "end",
};

Phases::Phases()
{
    values.reserve(names.size());
    for (unsigned long i = 0; i < names.size(); ++i)
        values.emplace_back(std::make_pair(false, 0));
    current = -1; // phase -1 is "press a button to start"
    portName = "";
    ip = "";
    baudrate = -1;
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
    qDebug() << "port: " << portName;
    qDebug() << "baudrate: " << baudrate;
    qDebug() << "ip: " << ip;
}

bool Phases::add(std::string nameStd, bool enabled, int number)
{
    qDebug() << "adding something";
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
        if (temp == "on") // enabled
            enabled = true;
        else if (temp == "off") // disabled
            enabled = false;
        else if (validBaudrate(temp)) // baud
            baudrate = std::stol(temp);
        else if (is_number(temp)) // number
            number = std::stoi(temp);
        else if (temp.find("/dev") != std::string::npos
                 || temp.find("com") != std::string::npos) // port
            portName = QString::fromStdString(temp);
        else if (validIp(temp)) // ip
            ip = QString::fromStdString(temp);
        else // name
            name = temp;
    }
    if (name == "baud" && baudrate == -1) {
        qDebug() << "invalid baud rate";
        return false;
    } else if (name == "ip" && ip == "") {
        qDebug() << "invalid baud rate";
        return false;
    }
    return this->add(name, enabled, number);

    // if the config contains an ip or a portname but "ip" or "port"
    // are spellt wrong their values are read anyway
}

bool Phases::is_number(const std::string &s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) {
                             return !std::isdigit(c);
                         }) == s.end();
}

bool Phases::validBaudrate(std::string s)
{
    return s == "1200" || s == "2400" || s == "4800" || s == "9600" || s == "19200" || s == "38400"
           || s == "57600" || s == "115200";
}

bool Phases::validIp(std::string s)
{
    int start = 0, occurrences = 0;
    while ((start = s.find('.', start)) != std::string::npos) {
        ++occurrences;
        ++start;
    }
    return occurrences == 3;
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
