//
// Created by robin on 29/04/2021.
//

#include "header/Vehicle.h"



Vehicle::Vehicle(float center_x,
                 float center_y,
                float width,
             float height,
             float rotation,
             std::string type,
             std::string name) :
             Collidable(center_x, center_y, width, height,rotation, name, type){}

void Vehicle::resetVisited() {
    int n =visited_[0].size();
    for(int i = 0; i<n ; i++){
        for(int k = 0; k<n ; k++){
            visited_[i][k]=0;
        }
    }
}

void Vehicle::resetPath() {
    path_.clear();
}

void Vehicle::diffDrive(Eigen::Vector2f v){
    float rad = rotation_ / 180 * M_PI;
    float r = 0;
    float omega = (v[1]-v[0])/width_;
    float x_prime = center_x_;
    float y_prime = center_y_;
    float rotprime = rad;
    // straight
    if((v[0]==v[1])){
        x_prime = center_x_ + v[0] * cos(rad);
        y_prime = center_y_ + v[0] * sin(rad);
    }
        // rotate in place
    else if (v[0] == -v[1]){
        rotprime = rad+omega;
    }
        // rotation about a wheel
    else {
        r = (v[0]+v[1])/(v[1]-v[0])*width_/2;
        Eigen::Matrix<float,3,3> rot;
        rot <<  cos(omega), -1*sin(omega),   0,
                sin(omega), cos(omega),      0,
                0,          0,               1;
        float icc_x = center_x_ - r * sin(rad);
        float icc_y = center_y_ + r * cos(rad);
        Eigen::Vector3f offset = {center_x_-icc_x, center_y_-icc_y, rad};
        Eigen::Vector3f turningPoint = {icc_x, icc_y, omega};
        Eigen::Vector3f prime = rot*offset+turningPoint;
        x_prime = prime[0];
        y_prime = prime[1];
        rotprime = prime[2];
    }
    center_x_=x_prime;
    center_y_=y_prime;
    rotation_=rotprime*180/M_PI;
}
