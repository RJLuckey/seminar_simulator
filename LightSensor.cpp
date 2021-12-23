//
// Created by robin on 30/04/2021.
//

#include "header/LightSensor.h"
#include "header/LightSource.h"
#include "math.h"
#include <algorithm>

LightSensor::LightSensor(LightSource* light, std::string name) : light_(light), name_(name){}

float LightSensor::getValueAt(float currentX, float currentY) {
    float distance = 0;
    float totalLight = 0;


    Eigen::Vector2f dist = light_->getPosition()-Eigen::Vector2f(currentX,currentY);
    distance = dist.norm();
    totalLight = std::max(0.0f,light_->getIntensity() - distance);


    //only for drawing purposes
    value_=totalLight;
    return totalLight;
}
