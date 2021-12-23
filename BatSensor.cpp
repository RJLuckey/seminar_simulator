//
// Created by moger on 24/06/2021.
//

#include "header/BatSensor.h"

float BatSensor::getLevel() {
    return bat_other->level_;
}

BatSensor::BatSensor(float offsetX, float offsetY, const Battery *batOther) : Sensor(offsetX, offsetY),
                                                                              bat_other(batOther) {}



