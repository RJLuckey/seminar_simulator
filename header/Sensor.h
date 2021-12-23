//
// Created by robin on 29/04/2021.
//

#ifndef EVO_SENSOR_H
#define EVO_SENSOR_H

#include <vector>

class Sensor {
public:
    Sensor(float offsetX, float offsetY);

public:

private:
    float offsetX_;
    float offsetY_;
};


#endif //EVO_SENSOR_H
