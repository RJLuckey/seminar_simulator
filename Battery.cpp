//
// Created by moger on 24/06/2021.
//

#include "header/Battery.h"

float Battery::getDepletionRate() const {
    return depletionRate_;
}

void Battery::setDepletionRate(float depletionRate) {
    depletionRate_ = depletionRate;
}

float Battery::getCapacity() const {
    return capacity_;
}

void Battery::setCapacity(float capacity) {
    capacity_ = capacity;
}

float Battery::getLevel() const {
    return level_;
}

void Battery::setLevel(float level) {
    Battery::level_ = level;
}

Battery::Battery(float cap, float rate) : capacity_(cap),depletionRate_(rate){}
