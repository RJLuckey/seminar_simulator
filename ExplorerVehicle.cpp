//
// Created by robin on 24/06/2021.
//

#include <chrono>
#include "header/ExplorerVehicle.h"

ProxSensor &ExplorerVehicle::getProxLeft() const {
    return proxLeft_;
}

void ExplorerVehicle::setProxLeft(ProxSensor &proxLeft) {
    proxLeft_ = proxLeft;
}

ProxSensor &ExplorerVehicle::getProxMid() const {
    return proxMid_;
}

void ExplorerVehicle::setProxMid(ProxSensor &proxMid) {
    proxMid_ = proxMid;
}

ProxSensor &ExplorerVehicle::getProxRight() const {
    return proxRight_;
}

void ExplorerVehicle::setProxRight(ProxSensor &proxRight) {
    proxRight_ = proxRight;
}

LightSensor &ExplorerVehicle::getLightFront() const {
    return lightFront_;
}

void ExplorerVehicle::setLightFront(LightSensor &lightFront) {
    ExplorerVehicle::lightFront_ = lightFront;
}

LightSensor &ExplorerVehicle::getLightBack() const {
    return lightBack_;
}

void ExplorerVehicle::setLightBack(LightSensor &lightBack) {
    ExplorerVehicle::lightBack_ = lightBack;
}
void ExplorerVehicle::updateRays(){
    double dec2rad = 1.0 / (180 / M_PI);
    float reach = 0.1;
    leftRay_[0] = center_x_ + cos((rotation_ + 90) * dec2rad) * reach;
    leftRay_[1] = center_y_ + sin((rotation_ + 90) * dec2rad) * reach;
    midRay_[0] = center_x_ + cos((rotation_) * dec2rad) * reach;
    midRay_[1] = center_y_ + sin((rotation_) * dec2rad) * reach;
    rightRay_[0] = center_x_ + cos((rotation_ - 90) * dec2rad) * reach;
    rightRay_[1] = center_y_ + sin((rotation_ - 90) * dec2rad) * reach;
}
bool ExplorerVehicle::control() {

        bb_ = this->getBoundingBox();

        updateRays();


        plv_ = proxLeft_.getValueAt(center_x_, center_y_, leftRay_);
        pmv_ = proxMid_.getValueAt(center_x_, center_y_, midRay_);
        prv_ = proxRight_.getValueAt(center_x_, center_y_, rightRay_);

        lightFrontX_ = bb_[1][0]-(bb_[1][0]-bb_[2][0])/2.0f;
        lightFrontY_ =bb_[1][1]-( bb_[1][1]-bb_[2][1])/2.0f;
        lightBackX_ =bb_[3][0]-(bb_[3][0]-bb_[0][0])/2.0f;
        lightBackY_ = bb_[3][1]-(bb_[3][1]-bb_[0][1])/2.0f;

        lf_ = lightFront_.getValueAt(lightFrontX_, lightFrontY_);
        lb_ = lightBack_.getValueAt(lightBackX_, lightBackY_);

        bluelf_ = blueLightFront_.getValueAt(bb_[1][0], bb_[1][1]);
        bluelb_ = blueLightBack_.getValueAt(bb_[2][0], bb_[2][1]);


        v_={0,0};
    if(battery_.level_ > 0) {
        battery_.setLevel((battery_.level_ - battery_.depletionRate_));
        v_ = controlMotors(plv_, pmv_, prv_,lf_, lb_,  bluelf_, bluelb_, battery_.getLevel());
        v_[0] = std::min(v_[0], 0.02f);
        v_[0] = std::max(v_[0], 0.0f);
        v_[1] = std::min(v_[1], 0.02f);
        v_[1] = std::max(v_[1], 0.0f);

        float oldCenterX = center_x_;
        float oldCenterY = center_y_;
        diffDrive(v_);


        bool collided = checkForCollisions();
        if (collided) {
            center_x_ = oldCenterX;
            center_y_ = oldCenterY;
            v_[0]=0;
            v_[1]=0;
        }
        //stdev::cout << battery_.level_ << stdev::endl;
        return collided;
    }
    else{
        v_[0]=0;
        v_[1]=0;
        return true;
    }
}

Eigen::Vector2f ExplorerVehicle::controlMotors(float sl, float sm, float sr, float lf, float lb, float batLevel, float blf, float blb) {

    std::vector<float> input = { sl,  sm,  sr, lf, lb, batLevel, blf, blb};
    std::vector<float>output = genome_.calcOutput(input);
    Eigen::Vector2f v;

    normal_distribution<float> disWiggle(0,0.001);
    long unsigned int ns = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine g{ns};

    v[0]= output[0];// + disWiggle(g);
    v[1]= output[1];// + disWiggle(g);

    return v;
}





ExplorerVehicle::ExplorerVehicle(float centerX, float centerY, float width, float height, float rotation,
                                 const string &name, ProxSensor &proxLeft, ProxSensor &proxMid,
                                 ProxSensor &proxRight, LightSensor &lightFront, LightSensor &lightBack, LightSensor &blueLightFront,LightSensor &blueLightBack,
                                 LightSource* redLight, Battery &battery,const Ann &genome) : Vehicle(centerX, centerY, width, height, rotation, "explorerVehicle", name),
                                                      proxLeft_(proxLeft), proxMid_(proxMid), proxRight_(proxRight),
                                                      lightFront_(lightFront), lightBack_(lightBack),blueLightFront_(blueLightBack),blueLightBack_(blueLightFront),
                                                      redLight_(redLight), battery_(battery),
                                                      genome_(genome) {
    redLight_->center_x_=center_x_;
    redLight_->center_y_=center_y_;
    bb_ = this->getBoundingBox();
}








