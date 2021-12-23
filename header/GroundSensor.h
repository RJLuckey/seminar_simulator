//
// Created by robin on 24/06/2021.
//

#ifndef MATPLOTLIB_CPP_GROUNDSENSOR_H
#define MATPLOTLIB_CPP_GROUNDSENSOR_H

#include "../constants.h"
#include "Sensor.h"

class GroundSensor: Sensor {
public:
    GroundSensor(float offsetX, float offsetY);

    float getValueAt(float x, float y);
};


#endif //MATPLOTLIB_CPP_GROUNDSENSOR_H
