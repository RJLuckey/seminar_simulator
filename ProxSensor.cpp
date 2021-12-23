//
// Created by robin on 03/05/2021.
//

#include <iostream>
#include "header/ProxSensor.h"
#include <algorithm>
ProxSensor::ProxSensor(const std::vector<Collidable*> collidables) : collidables_(collidables) {}



float ProxSensor::getValueAt(float centerX,float centerY, Eigen::Vector2f ray) {
    float dist = std::numeric_limits<float>::infinity();
    for(auto coll: collidables_){
        std::vector<Eigen::Vector2f> bb = coll->bb_;

        float t = -1;
        float u = -1;

        for(int i=0; i<bb.size(); i++){
            auto current = bb[i];
            auto next = bb[(i+1)%bb.size()];
            float x1=centerX;
            float y1=centerY;
            float x2=ray[0];
            float y2=ray[1];
            float x3=current[0];
            float y3=current[1];
            float x4=next[0];
            float y4=next[1];
            float t_num = (x1-x3)*(y3-y4)-(y1-y3)*(x3-x4);
            float u_num =(x1-x2)*(y1-y3)-(y1-y2)*(x1-x3);
            float denom = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
            t=t_num/denom;
            u=-1*u_num/denom;
            if(!(t>=1||t<=0||u>=1||u<=0)){
                Eigen::Vector2f intersection =  {x1+t*(x2-x1),y1+t*(y2-y1)};
                dist = std::min(dist,(intersection - Eigen::Vector2f(centerX,centerY)).norm());
            }

        }

    }
    return dist==std::numeric_limits<float>::infinity()? 0 : (1-dist);
}
