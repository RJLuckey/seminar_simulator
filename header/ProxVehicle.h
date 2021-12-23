//
// Created by robin on 04/05/2021.
//

#ifndef EVO_PROXVEHICLE_H
#define EVO_PROXVEHICLE_H


#include "Vehicle.h"
#include "ProxSensor.h"
#include "Genome.h"
#include "../network/Ann/Ann.h"
#include "types.h"

class ProxVehicle: public Vehicle{
private:
    ProxSensor& proxLeft_;
    ProxSensor& proxMid_;
    ProxSensor& proxRight_;

    Eigen::Vector2f controlMotors(float plv, float pmv, float prv);
public:
    Controller genome_;
    Eigen::Vector2f leftRay_;
    Eigen::Vector2f midRay_;
    Eigen::Vector2f rightRay_;
    float plv_=0;
    float plv_track=0;
    float prv_track=0;
    float pmv_=0;
    float prv_=0;
    Eigen::Vector2f v_={0,0};
    bool control(int mode = -1, int randomoffset = 0) ;
    ProxVehicle(float centerX, float centerY, float width, float height, float rotation,
                ProxSensor &proxLeft, ProxSensor &proxMid, ProxSensor &proxRight, std::string name,
                Controller genome);


    void setGenome(Ann genome);

    void setPosition(float center_x, float center_y, float rotation );

};



#endif //EVO_PROXVEHICLE_H
