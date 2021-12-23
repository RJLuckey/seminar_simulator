//
// Created by robin on 29/04/2021.
//

#ifndef EVO_PLACEABLE_H
#define EVO_PLACEABLE_H


#include <string>

class Placeable {
public:
    float center_x_;
    float center_y_;
    std::string name_;
    std::string type_;
public:
    Placeable(float center_x, float center_y, std::string name, std::string type);

};


#endif //EVO_PLACEABLE_H
