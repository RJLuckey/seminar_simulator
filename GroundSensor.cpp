//
// Created by robin on 24/06/2021.
//

#include "header/GroundSensor.h"
#include <math.h>
float GroundSensor::getValueAt(float x, float y) {
    return  sqrt(x*x + y*y)<chargerRadius;
}

GroundSensor::GroundSensor(float offsetX, float offsetY) : Sensor(offsetX, offsetY) {}

