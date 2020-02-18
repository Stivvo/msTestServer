#include "phases.h"

Phases::Phases() {
    names = {"start", "touch", "brightness", "turnoffScreen", "usb", "red", "green", "blue", "black", "white", "end"};
    current = -1; // phase -1 is "press a button to start"
}

bool Phases::advance() {
    if (current < names.size() || current == -1) {
        current++;
        return true;
    }
    return false;
}

bool Phases::isLast() {
    return current == this->size() - 1;
}

bool Phases::finished() {
    return current == this->size();
}

bool Phases::isFirst() {
    return current == -1;
}

bool Phases::add(std::string nameStd, bool enabled, int number) {
    bool ret = false;
    int i = 0;
    QString name = QString::fromStdString(nameStd); // costava tanto a qt fare un costruttore con parametri per qstring che prendeva std::string
    while (i < names.size() && !ret) {
        if (names.at(i) == name) {
            ret = true;
            values.at(i) = std::make_pair(enabled, number);
        }
    }
    return ret;
}

QString Phases::currentName() {
    return names.at(current);
}

bool Phases::currentEnabled() {
    return values.at(current).first;
}

int Phases::currentNumber() {
    return values.at(current).second;
}
