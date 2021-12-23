//
// Created by robin on 24/06/2021.
//

#ifndef MATPLOTLIB_CPP_CHARGEVEHICLE_H
#define MATPLOTLIB_CPP_CHARGEVEHICLE_H


#include "Vehicle.h"
#include "ProxSensor.h"
#include "LightSensor.h"
#include "../network/Ann/Ann.h"
#include "Battery.h"
#include "BatSensor.h"
#include "GroundSensor.h"

class ChargeVehicle : public Vehicle{
public:
    Eigen::Vector2f leftRay_;
    Eigen::Vector2f rightRay_;
    Eigen::Vector2f midRay_;
    ProxSensor& proxLeft_;
    ProxSensor& proxMid_;
    ProxSensor& proxRight_;
    LightSensor& lightFront_;
    LightSensor& lightBack_;
    LightSensor& redLightFront_;
    LightSensor& redLightBack_;
    LightSource* blueLight_;
    GroundSensor& groundSensor_;
    Battery& battery_;
    Battery& rechargeBattery_;
    BatSensor& batSensor_;

    float plv_ =0;
    float pmv_ =0;
    float prv_ =0;
    float lf_ =0;
    float lb_ =0;

    float redlf_=0;
    float redlb_=0;


    float lightFrontX_ = 0;
    float lightFrontY_ = 0;
    float lightBackX_ = 0;
    float lightBackY_ =0;

    float otherBatLevel_;



    ChargeVehicle(float centerX, float centerY, float width, float height, float rotation,
                  const string &name, ProxSensor &proxLeft, ProxSensor &proxMid, ProxSensor &proxRight,
                  LightSensor &lightFront, LightSensor &lightBack, LightSensor &redLightFront,  LightSensor &redLightBack, GroundSensor &groundSensor, LightSource* blueLight, Battery &battery,Battery &rechargeBattery_,
                  BatSensor &batSensor, const Ann &genome);



private:
    Ann genome_;
    Eigen::Vector2f controlMotors(float plv, float pmv, float prv, float lf, float lb, float redlf, float redlb, float gr, float batLevel, float otherBatLevel);
public:


    Battery &getBattery() const;

    void setBattery(Battery &battery);

    BatSensor &getBatSensor() const;

    void setBatSensor(BatSensor &batSensor);
    ProxSensor &getProxLeft() const;

    void setProxLeft(ProxSensor &proxLeft);

    ProxSensor &getProxMid() const;

    void setProxMid(ProxSensor &proxMid);

    ProxSensor &getProxRight() const;

    void setProxRight(ProxSensor &proxRight);

    LightSensor &getLightFront() const;

    void setLightFront(LightSensor &lightFront);

    LightSensor &getLightBack() const;

    void setLightBack(LightSensor &lightBack);

    bool control();

    void updateRays();
};


#endif //MATPLOTLIB_CPP_CHARGEVEHICLE_H
