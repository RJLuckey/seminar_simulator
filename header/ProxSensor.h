//
// Created by robin on 03/05/2021.
//

#ifndef EVO_PROXSENSOR_H
#define EVO_PROXSENSOR_H


#include "Collidable.h"

class ProxSensor {
private:
    std::vector<Collidable*> collidables_;
public:

    ProxSensor(const std::vector<Collidable*> collidables);

    float getValueAt(float currentX,float currentY, Eigen::Vector2f ray);
};


#endif //EVO_LIGHTSENSOR_H
