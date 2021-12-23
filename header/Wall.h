//
// Created by robin on 29/04/2021.
//

#ifndef EVO_WALL_H
#define EVO_WALL_H


#include <opencv2/core/types.hpp>
#include "Collidable.h"

class Wall: public Collidable {

public:
    std::string type_="wall";
    cv::Scalar color_ = cv::Scalar(255, 200, 150);
    Wall(float centerX, float centerY, float width, float height, float rotation, std::string name);

};


#endif //EVO_WALL_H
