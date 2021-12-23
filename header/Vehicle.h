//
// Created by robin on 29/04/2021.
//

#ifndef EVO_VEHICLE_H
#define EVO_VEHICLE_H


#include <vector>
#include <opencv2/core/types.hpp>
#include "Collidable.h"
#include "Controllable.h"
#include "Sensor.h"
#include "Placeable.h"
#include "../constants.h"

class Vehicle: public Collidable, public Controllable {
public:

    Eigen::Vector2f v_ = {0,0};
    std::array<std::array<short,cellsDim>,cellsDim>visited_;
    std::vector<cv::Point3f> path_;
    void resetVisited();
    void resetPath();
    void diffDrive(Eigen::Vector2f v);
    Vehicle(float center_x,float center_y,float width, float height, float rotation, std::string type, std::string name);

};


#endif //EVO_VEHICLE_H
