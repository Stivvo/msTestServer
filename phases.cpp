#include "phases.h"

Phases::Phases() {
    names = {"start", "touch", "brightness", "turnoffScreen", "usb", "red", "green", "blue", "black", "white", "end"};
}

void Phases::advance() {
    if (current < names.size())
        current++;
}

bool Phases::add(std::string name, bool enabled, int number) {
    bool ret = false;
    int i = 0;
    while (i < names.size() && !ret) {
        if (names.at(i) == name) {
            ret = true;
            values.at(i) = std::make_pair(enabled, number);
        }
    }
    return ret;
}

std::string Phases::currentName() {
    return names.at(current);
}

bool Phases::currentEnabled() {
    return values.at(current).first;
}

int Phases::currentNumber() {
    return values.at(current).second;
}
