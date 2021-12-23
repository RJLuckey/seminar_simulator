//
// Created by robin on 30/04/2021.
//

#ifndef EVO_LIGHTSENSOR_H
#define EVO_LIGHTSENSOR_H


#include <vector>
#include "Sensor.h"
#include "LightSource.h"

class LightSensor {
private:
    //only for drawing purposes

public:
    float value_;
    std::string name_;
    LightSource* light_;
    LightSensor(LightSource* light,std::string name);

    float getValueAt(float currentX, float currentY);
};


#endif //EVO_LIGHTSENSOR_H
