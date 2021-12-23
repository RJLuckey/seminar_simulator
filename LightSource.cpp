//
// Created by robin on 29/04/2021.
//

#include <iostream>
#include "header/LightSource.h"
#include "header/Eigen/Core"

float LightSource::getIntensity(){
    return intensity_;
}

std::string LightSource::getName() const {
    return name_;
}



LightSource::LightSource(float center_x,
                         float center_y,
                         float intensity,
                         const std::string name) :
                         Placeable(center_x,center_y, name, "lightSource"),
                         intensity_(intensity),
                         name_(name){
}

Eigen::Vector2f  LightSource::getPosition() {
    return {center_x_, center_y_};;
}