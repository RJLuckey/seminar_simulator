//
// Created by moger on 24/06/2021.
//

#ifndef MATPLOTLIB_CPP_BATSENSOR_H
#define MATPLOTLIB_CPP_BATSENSOR_H

#include "Battery.h"
#include "Sensor.h"
#include "ExplorerVehicle.h"

class BatSensor: Sensor {
public:
    BatSensor(float offsetX, float offsetY, const ExplorerVehicle* explorerVehicle);

    float getLevel();
    const Battery* bat_other;

    BatSensor(float offsetX, float offsetY, const Battery *batOther);
};


#endif //MATPLOTLIB_CPP_BATSENSOR_H
