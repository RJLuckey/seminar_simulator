//
// Created by robin on 29/04/2021.
//

#ifndef EVO_COLLIDABLE_H
#define EVO_COLLIDABLE_H

#include "Eigen/Core"
#include "Placeable.h"



class Collidable: public Placeable {
public:
    float width_;
    float height_;
    float rotation_;
    float xOffset_;
    std::vector<Eigen::Vector2f> bb_;

    Collidable(float center_x, float center_y, float width, float height, float rotation, std::string name, std::string type);

    float getWidth() const;
    float getHeight() const;
    float getRotation() const;
    bool intersects(std::vector<Eigen::Vector2f> bbOther, std::string name);

    virtual std::vector<Eigen::Vector2f> getBoundingBox() ;
protected:
    std::vector<Collidable*> allCollidables_;
public:
    void setAllCollidables(const std::vector<Collidable *> &allCollidables);
    bool checkForCollisions();

};


#endif //EVO_COLLIDABLE_H
