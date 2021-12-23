//
// Created by robin on 24/06/2021.
//

#include <chrono>
#include "header/ChargeVehicle.h"

ProxSensor &ChargeVehicle::getProxLeft() const {
    return proxLeft_;
}

void ChargeVehicle::setProxLeft(ProxSensor &proxLeft) {
    proxLeft_ = proxLeft;
}

ProxSensor &ChargeVehicle::getProxMid() const {
    return proxMid_;
}

void ChargeVehicle::setProxMid(ProxSensor &proxMid) {
    proxMid_ = proxMid;
}

ProxSensor &ChargeVehicle::getProxRight() const {
    return proxRight_;
}

void ChargeVehicle::setProxRight(ProxSensor &proxRight) {
    proxRight_ = proxRight;
}

LightSensor &ChargeVehicle::getLightFront() const {
    return lightFront_;
}

void ChargeVehicle::setLightFront(LightSensor &lightFront) {
    ChargeVehicle::lightFront_ = lightFront;
}

LightSensor &ChargeVehicle::getLightBack() const {
    return lightBack_;
}

void ChargeVehicle::setLightBack(LightSensor &lightBack) {
    ChargeVehicle::lightBack_ = lightBack;
}

void ChargeVehicle::updateRays(){
    double dec2rad = 1.0 / (180 / M_PI);
    float reach = 0.1;
    leftRay_[0] = center_x_ + cos((rotation_ + 90) * dec2rad) * reach;
    leftRay_[1] = center_y_ + sin((rotation_ + 90) * dec2rad) * reach;
    midRay_[0] = center_x_ + cos((rotation_) * dec2rad) * reach;
    midRay_[1] = center_y_ + sin((rotation_) * dec2rad) * reach;
    rightRay_[0] = center_x_ + cos((rotation_ - 90) * dec2rad) * reach;
    rightRay_[1] = center_y_ + sin((rotation_ - 90) * dec2rad) * reach;
}

bool ChargeVehicle::control() {


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
        redlf_ = redLightFront_.getValueAt(bb_[1][0], bb_[1][1]);
        redlb_ = redLightBack_.getValueAt(bb_[2][0], bb_[2][1]);
        otherBatLevel_ = batSensor_.getLevel();
        float gr = groundSensor_.getValueAt(center_x_,center_y_);


        v_={0,0};
    if(battery_.level_ > 0) {
        battery_.setLevel((battery_.level_ - battery_.depletionRate_));
        v_ = controlMotors(plv_, pmv_, prv_,lf_, lb_, gr, battery_.getLevel(), redlf_, redlb_, otherBatLevel_);
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
        return collided;
    }
    else{
        v_[0]=0;
        v_[1]=0;
        return true;
    }
}

Eigen::Vector2f ChargeVehicle::controlMotors(float sl, float sm, float sr, float lf, float lb, float redlf, float redlb, float gr, float batLevel, float otherBatLevel) {

    std::vector<float> input = { sl,  sm,  sr,  lf,  lb,  redlf,  redlb,  gr,  batLevel,  otherBatLevel};
    std::vector<float>output = genome_.calcOutput(input);
    Eigen::Vector2f v;

    normal_distribution<float> disWiggle(0,0.001);
    long unsigned int ns = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine g{ns};

    v[0]= output[0]; //+ disWiggle(g);
    v[1]= output[1];// + disWiggle(g);

    return v;
}





ChargeVehicle::ChargeVehicle(float centerX, float centerY, float width, float height, float rotation,
                             const string &name, ProxSensor &proxLeft,
                             ProxSensor &proxMid, ProxSensor &proxRight, LightSensor &lightFront,
                             LightSensor &lightBack, LightSensor &redLightFront,  LightSensor &redLightBack, GroundSensor &groundSensor, LightSource* blueLight, Battery &battery, Battery &rechargeBatCH, BatSensor &batSensor,
                             const Ann &genome) : Vehicle(centerX, centerY, width, height, rotation, "chargeVehicle", name),
                                                  proxLeft_(proxLeft), proxMid_(proxMid), proxRight_(proxRight),
                                                  lightFront_(lightFront), lightBack_(lightBack), redLightFront_(redLightFront),  redLightBack_(redLightBack),
                                                  groundSensor_(groundSensor), blueLight_(blueLight), battery_(battery), rechargeBattery_(rechargeBatCH), batSensor_(batSensor),
                                                  genome_(genome) {
    bb_ = this->getBoundingBox();
    blueLight_->center_x_=center_x_;
    blueLight_->center_y_=center_y_;
}


