//
// Created by robin on 29/04/2021.
//

#include "header/Wall.h"


Wall::Wall(float centerX, float centerY, float width, float height, float rotation, std::string name) : Collidable(centerX, centerY,
                                                                                                 width, height,
                                                                                                 rotation, name, "wall") {
    this->bb_=getBoundingBox();
}
