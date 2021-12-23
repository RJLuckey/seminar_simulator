//
// Created by robin on 29/04/2021.
//

#include "header/Collidable.h"
#include "header/Eigen/Core"
#include <math.h>
float Collidable::getWidth() const {
    return width_;
}

float Collidable::getHeight() const {
    return height_;
}

float Collidable::getRotation() const {
    return rotation_;
}




Collidable::Collidable(float center_x, float center_y, float width, float height, float rotation, std::string name, std::string type) : Placeable(center_x,center_y, name, type), width_(width), height_(height), rotation_(rotation) {}



std::vector<Eigen::Vector2f> Collidable::getBoundingBox() {
    float rad = rotation_*M_PI / 180;
    Eigen::Matrix<float,2,2> rot;
    rot <<  cos(rad),-1*sin(rad),
            sin(rad),cos(rad);

    Eigen::Vector2f topLeft={-width_/2,height_/2};
    Eigen::Vector2f bottomRight={width_/2,-height_/2};
    Eigen::Vector2f topRight={width_/2,height_/2};
    Eigen::Vector2f bottomLeft={-width_/2,-height_/2};
    topLeft = rot*topLeft;
    bottomRight = rot*bottomRight;
    topRight = rot*topRight;
    bottomLeft = rot*bottomLeft;
    Eigen::Vector2f center = {center_x_,center_y_};
    topLeft +=center;
    bottomRight +=center;
    topRight +=center;
    bottomLeft +=center;
    std::vector<Eigen::Vector2f> bb;
    bb.push_back(topLeft);
    bb.push_back(topRight);
    bb.push_back(bottomRight);
    bb.push_back(bottomLeft);

    return bb;
}
bool findSeparation(std::vector<Eigen::Vector2f> bb, std::vector<Eigen::Vector2f> bbOther, std::string name) {

    // In this method we will be calculating whether or not a given collidable is intersecting with another collidable
    // to do this we will be using the sat(separating axis theorem)


    for(auto i = 0; i < bb.size() ; i++){
        //first we need to calculate the axis for the axis we are currently looking at
        Eigen::Vector2f current = bb[i];
        Eigen::Vector2f next = bb[(i+1)%bb.size()];
        Eigen::Vector2f edge = next - current;

        Eigen::Vector2f axis = {-edge[1], edge[0]};
        float vMaxProj = -std::numeric_limits<float>::infinity();
        float vMinProj = std::numeric_limits<float>::infinity();
        float oMaxProj = -std::numeric_limits<float>::infinity();
        float oMinProj = std::numeric_limits<float>::infinity();
        for(Eigen::Vector2f a : bb){
            auto proj = axis.dot(a)/axis.norm();
            if(proj < vMinProj) vMinProj = proj;
            if(proj > vMaxProj) vMaxProj = proj;
        }

        for(Eigen::Vector2f  a : bbOther){
            float proj = axis.dot(a)/axis.norm();
            if(proj < oMinProj) oMinProj = proj;
            if(proj > oMaxProj) oMaxProj = proj;
        }
        if(vMaxProj < oMinProj || vMinProj > oMaxProj)
            return true;
        }


    return false;
}
bool Collidable::intersects(std::vector<Eigen::Vector2f> bbOther, std::string name) {
    return !findSeparation(bbOther, bb_, name);
}
void Collidable::setAllCollidables(const std::vector<Collidable *> &allCollidables) {
    Collidable::allCollidables_ = allCollidables;
}

bool Collidable::checkForCollisions(){
    bool doesCollide = false;
    for (auto c : allCollidables_){
        if(c->name_ != name_ && intersects(c->bb_, c->name_)){
            doesCollide= true;
            break;
        }

    }
    return doesCollide;
}

