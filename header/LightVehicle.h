//
// Created by robin on 29/04/2021.
//

#ifndef EVO_LIGHTVEHICLE_H
#define EVO_LIGHTVEHICLE_H


#include "Vehicle.h"
#include "LightSensor.h"

class LightVehicle: public Vehicle{
public:
    bool isScared_;


public:
    LightSensor& lightLeft_;
    LightSensor& lightRight_;

    LightVehicle(float centerX, float centerY, float width, float height, float rotation, bool isScared,
                 LightSensor &lightLeft, LightSensor &lightRight, std::string name);

private:
    bool control();
};


#endif //EVO_LIGHTVEHICLE_H
