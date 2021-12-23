//
// Created by robin on 29/04/2021.
//

#ifndef EVO_LIGHTSOURCE_H
#define EVO_LIGHTSOURCE_H



#include <iostream>
#include "Placeable.h"
#include "Eigen/Core"

class LightSource: public Placeable {
private:
    float intensity_;
    std::string name_;
public:
    LightSource(float center_x, float center_y, float intensity, const std::string name);

    std::string getName() const;
    float getIntensity();
    Eigen::Vector2f getPosition() ;
};


#endif //EVO_LIGHTSOURCE_H
