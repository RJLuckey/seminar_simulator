//
// Created by robin on 29/04/2021.
//

#include "header/LightVehicle.h"
#include <algorithm>

LightVehicle::LightVehicle(float centerX, float centerY, float width, float height, float rotation, bool isScared,
                           LightSensor &lightLeft, LightSensor &lightRight, std::string name) : Vehicle(centerX, centerY, width, height,
                                                                                      rotation,"lightVehicle", name), isScared_(isScared),
                                                                              lightLeft_(lightLeft),
                                                                              lightRight_(lightRight) {}

bool LightVehicle::control() {
    bb_ = this->getBoundingBox();
    float ll = lightLeft_.getValueAt(bb_[1][0], bb_[1][1]);
    float lr = lightRight_.getValueAt(bb_[2][0], bb_[2][1]);
    float vl =ll;
    float vr =lr;
    if(isScared_){
        rotation_ +=30*(vr-vl);
    }else{
        rotation_ += 30*(vl-vr);
    }

    float speed = 0.05*(vl+vr)/2;
    if(speed<0.02)speed = 0.01;
    if(speed>0.05)speed = 0.2;
    center_x_= center_x_+speed*cos(rotation_/180*M_PI);
    center_y_= center_y_+speed*sin(rotation_/180*M_PI);
    if(rotation_>360)rotation_-=360;
    if(rotation_<-360)rotation_+=360;
    if(center_x_>1)center_x_-=1;
    if(center_y_>1)center_y_-=1;
    if(center_x_<0)center_x_+=1;
    if(center_y_<0)center_y_+=1;
    return false;
}

