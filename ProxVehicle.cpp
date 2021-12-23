//
// Created by robin on 04/05/2021.
//

#include <iostream>
#include <chrono>
#include "header/ProxVehicle.h"
#include "header/Wall.h"


bool ProxVehicle::control(int mode, int randomoffset) {
    double dec2rad = 1.0/(180/M_PI);
    float reach = 0.1;
    bb_ = this->getBoundingBox();

    leftRay_[0] = center_x_  + cos((rotation_+45)*dec2rad)*reach;
    leftRay_[1] = center_y_ + sin((rotation_+45)*dec2rad)*reach;
    midRay_[0] = center_x_  + cos((rotation_)*dec2rad)*reach;
    midRay_[1] = center_y_ + sin((rotation_)*dec2rad)*reach;
    rightRay_[0] = center_x_  + cos((rotation_-45)*dec2rad)*reach;
    rightRay_[1] = center_y_ + sin((rotation_-45)*dec2rad)*reach;


    plv_ = proxLeft_.getValueAt(center_x_, center_y_, leftRay_);
    pmv_ = proxMid_.getValueAt(center_x_, center_y_, midRay_);
    prv_ = proxRight_.getValueAt(center_x_, center_y_, rightRay_);

    plv_track = plv_;
    prv_track = prv_;
    long unsigned int ns = chrono::system_clock::now().time_since_epoch().count();
    if(mode == LEFT_BROKEN){
        ns = 0+randomoffset;
        plv_track = plv_;
        plv_ = 0;
    }
    if(mode == RIGHT_BROKEN){
        ns = 0+randomoffset;
        prv_ = 0;
    }
    if(mode == 3){
        ns = 0;
        plv_ = 0;
        pmv_ = 0;
        prv_ = 0;
    }

    v_ = controlMotors(plv_, pmv_, prv_);

    default_random_engine g{ns};
    normal_distribution<float> dis(0.0,NOICE);

    v_[0]=std::min(v_[0], 0.02f);
    v_[0]=std::max(v_[0], 0.0f);
    v_[1]=std::min(v_[1], 0.02f);
    v_[1]=std::max(v_[1], 0.0f);
    v_[0]+=dis(g);
    v_[1]+=dis(g);
    float oldCenterX = center_x_;
    float oldCenterY = center_y_;
    diffDrive(v_);


    bool collided = checkForCollisions();
    if(collided){
        center_x_=oldCenterX;
        center_y_=oldCenterY;
    }

    return collided;

}

Eigen::Vector2f ProxVehicle::controlMotors(float sl, float sm, float sr) {
    std::vector<float> input;
    input = { sl,  sm,  sr};
    std::vector<float>output = genome_.explorerAnn.calcOutput(input);
    Eigen::Vector2f v;
    v[0]= output[0];
    v[1]= output[1];



    return v;
}

ProxVehicle::ProxVehicle(float centerX, float centerY, float width, float height, float rotation,
                         ProxSensor &proxLeft, ProxSensor &proxMid, ProxSensor &proxRight, std::string name,
                         Controller genome) :
        Vehicle(centerX, centerY, width, height, rotation,"proxVehicle", name),
        proxLeft_(proxLeft), proxMid_( proxMid), proxRight_(proxRight), genome_(genome) {}



void ProxVehicle::setPosition(float center_x, float center_y, float rotation) {
    center_x_ = center_x;
    center_y_ = center_y;
    rotation_=rotation;
}
