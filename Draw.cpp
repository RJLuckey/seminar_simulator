//
// Created by robin on 02/05/2021.
//
#include <iostream>
#include "header/Draw.h"
#include "header/Vehicle.h"
#include "header/ProxVehicle.h"
#include "header/LightVehicle.h"
#include "header/Wall.h"
#include <math.h>
#define wwidth 1000
#include "constants.h"

void Draw::drawPop(std::array<Controller, POP_SIZE>pop){
    cv::Mat popMat = cv::Mat::zeros(100, POP_SIZE, CV_8UC3);
    for (int i = 0; i < POP_SIZE; ++i) {
        popMat.at<cv::Vec3b>(pop[i].fitness*1000,i ) = cv::Vec3b(255, 255, 255);
    }
    cv::imwrite("plots/graph_"+std::to_string(std::time(0))+".jpg", popMat);
}

void drawBB(std::vector<Eigen::Vector2f> bb, cv::Mat worldImage, cv::Scalar color){
    cv::Point ps[4];
    for (int i=0;i<4;i++){
    ps[i].x=bb[i][0]*wwidth;
    ps[i].y=bb[i][1]*wwidth;
    }
    cv::fillConvexPoly(worldImage, ps, 4, color);
}
void drawCaption(float x, float y, cv::Mat worldImage, cv::Scalar color, string text, int width){

    cv::putText(worldImage, text, cv::Point(x*width,y*width), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                color, 1, cv::LINE_AA, true);
}
void Draw::drawHeatmap(ElitesMap elMap, int gen, int counter){
    int scale = DESCIRPTOR_DIM*HEATMAP_SCALE+400;
    cv::Mat elitesMap = cv::Mat::zeros(scale, scale, CV_8UC3);
    for (int i = 0; i < DESCIRPTOR_DIM; ++i) {
        for (int n = 0; n < DESCIRPTOR_DIM; ++n) {
            for (int j = 0; j < DESCIRPTOR_DIM; ++j) {
                    Controller con = elMap.elites[i][j][n];
                    cv::Vec3b color;
                    float max_fitness = TIMESTEPS / 50;
                    if(con.fitness !=-1){
                        color = cv::Vec3b((1-con.fitness/max_fitness) * 255,
                                (con.fitness/max_fitness) * 255,
                                          100);
                    } else{
                        color = cv::Vec3b(255,255,255);
                    }
                    if(elMap.elites[i][j][n].fitness == elMap.bestController.fitness){
                        color = cv::Vec3b(0,0,0);
                    }

                    for (int k = 0; k < HEATMAP_SCALE; ++k) {
                        for (int l = 0; l < HEATMAP_SCALE-DESCIRPTOR_DIM; ++l) {
                            elitesMap.at<cv::Vec3b>((i * HEATMAP_SCALE) + j + l +200, (n * HEATMAP_SCALE)+k+200) = color;
                        }

                    }


            }
        }
    }
    drawCaption(0.35, 0.9, elitesMap, cv::Scalar(255, 255, 255), "Cols: Right Sensor, inner Rows: Middle Sensor", scale);
    drawCaption(0.05, 0.5, elitesMap, cv::Scalar(255, 255, 255), "Row: Left Sensor ", scale);
    drawCaption(0.05, 0.20, elitesMap, cv::Scalar(255, 255, 255), "Best Controller:", scale);
    drawCaption(0.05, 0.15, elitesMap, cv::Scalar(255, 255, 255), "Fitness: " + to_string(elMap.bestController.fitness), scale);
    drawCaption(0.05, 0.10,elitesMap,cv::Scalar(255, 255, 255), "l: " + to_string(elMap.bestController.l) + " m: " +  to_string(elMap.bestController.m) + " r: " + to_string(elMap.bestController.r),scale);
    drawCaption(0.05, 0.05,elitesMap,cv::Scalar(255, 255, 255),"Generation " + to_string(gen), scale);
    flip(elitesMap,elitesMap,0);
    cv::imshow("The Elites",elitesMap);
    cv::imwrite("../plots/"+std::to_string(DESCIRPTOR_DIM)+"_"+to_string(counter)+".jpg", elitesMap);
    cv::waitKey(10);
}
void drawRays(Controllable* con, cv::Mat worldImage, cv::Scalar color){
    ProxVehicle* pv =  (ProxVehicle*) con;
    cv::Point p1;
    cv::Point p2;
    p1.x=pv->center_x_*wwidth;
    p1.y=pv->center_y_*wwidth;
    p2.x=pv->leftRay_[0]*wwidth;
    p2.y=pv->leftRay_[1]*wwidth;
    cv::line(worldImage, p1, p2, color, 4);
    p2.x=pv->midRay_[0]*wwidth;
    p2.y=pv->midRay_[1]*wwidth;
    cv::line(worldImage, p1, p2, color, 4);
    p2.x=pv->rightRay_[0]*wwidth;
    p2.y=pv->rightRay_[1]*wwidth;
    cv::line(worldImage, p1, p2, color, 4);
}





void Draw::drawWorld(World *w, bool writeToFile, float fitness, int timestep, int gen) {
    cv::Mat worldImage = cv::Mat::zeros( wwidth, wwidth, CV_8UC3 );


    //draw walls
    std::vector<Collidable*> colls = w->getCollidables();
    for(auto coll : colls){
        if(coll->type_=="wall"){
            drawBB(coll->bb_, worldImage,cv::Scalar(255, 200, 150));
            //reset color
           // ((Wall*)coll)->color_=cv::Scalar(255, 200, 150);
        }else{
            drawBB(coll->bb_, worldImage,cv::Scalar(255, 200, 150));

        }

    }
    std::vector<Controllable*> cons = w->getControllables();
    for(auto con : cons){
        Vehicle* v = (Vehicle*)con;
        if(v->name_=="prox1"){
            ProxVehicle* exv = (ProxVehicle*) con;
            std::vector<Eigen::Vector2f> bb = exv->bb_;
            drawBB(bb, worldImage, cv::Scalar(0, 255, 255));
            drawRays(v, worldImage, cv::Scalar(0, 255, 0));
        }
    }
    //lights
    std::vector<Placeable*> ps = w->getPlaceables();
    for( auto p : ps) {
        cv::Scalar color;
        if(p->name_=="red"){
            color = cv::Scalar(50, 50, 255);
        }else if(p->name_=="blue"){
            color = cv::Scalar(255, 50, 50);
        } else{
            color = cv::Scalar(50, 255, 255);
        }
        circle(worldImage,
               cv::Point( p->center_x_*wwidth, p->center_y_*wwidth),
               10,
               color,
               cv::FILLED,
               cv::LINE_8);
    }

    drawCaption(0.1, 0.84, worldImage, cv::Scalar(255, 255, 255), "Generation: " + std::to_string(gen), wwidth);
    drawCaption(0.1, 0.82,worldImage,cv::Scalar(255, 255, 255), "Timestep: " + std::to_string(timestep), wwidth);
    drawCaption(0.1, 0.8, worldImage, cv::Scalar(255, 255, 255), "Fitness: " + std::to_string(fitness), wwidth);
    flip(worldImage,worldImage,0);

    if(writeToFile){
        cv::imwrite("plots/"+std::to_string(std::time(0))+".jpg", worldImage);

    }
    else{

        cv::imshow("The World",worldImage);
        cv::waitKey(10);

    }

}
