//
// Created by robin on 24/06/2021.
//

#ifndef MATPLOTLIB_CPP_EXPLORERVEHICLE_H
#define MATPLOTLIB_CPP_EXPLORERVEHICLE_H


#include "Vehicle.h"
#include "ProxSensor.h"
#include "LightSensor.h"
#include "../network/Ann/Ann.h"
#include "Battery.h"
#include "GroundSensor.h"

class ExplorerVehicle : public Vehicle{
public:
    Eigen::Vector2f leftRay_;
    Eigen::Vector2f rightRay_;
    Eigen::Vector2f midRay_;
    ProxSensor& proxLeft_;
    ProxSensor& proxMid_;
    ProxSensor& proxRight_;
    LightSensor& lightFront_;
    LightSensor& lightBack_;
    LightSensor& blueLightFront_;
    LightSensor& blueLightBack_;
    LightSource* redLight_;
    Battery& battery_;

    float plv_ =0;
    float pmv_ =0;
    float prv_ =0;
    float lf_ =0;
    float lb_ =0;

    float bluelf_ = 0;
    float bluelb_=0;

    float lightFrontX_ = 0;
    float lightFrontY_ = 0;
    float lightBackX_ = 0;
    float lightBackY_ =0;


    ExplorerVehicle(float centerX, float centerY, float width, float height, float rotation,
                    const string &name, ProxSensor &proxLeft, ProxSensor &proxMid, ProxSensor &proxRight,
                    LightSensor &lightFront, LightSensor &lightBack,  LightSensor &blueLightFront,  LightSensor &blueLightBack, LightSource* redLight, Battery &battery,
                    const Ann &genome);

private:
    Ann genome_;
    Eigen::Vector2f controlMotors(float sl, float sm, float sr, float lf, float lb, float batLevel, float blf, float blb);
public:
    void updateRays();
    Battery &getBattery() const;

    void setBattery(Battery &battery);

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
};


#endif //MATPLOTLIB_CPP_EXPLORERVEHICLE_H
