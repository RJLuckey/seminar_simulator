//
// Created by r on 07.11.21.
//

#ifndef MATPLOTLIB_CPP_TYPES_H
#define MATPLOTLIB_CPP_TYPES_H
#include "../network/Ann/Ann.h"
#include "../constants.h"
#include <array>

#include "../header/Eigen/Core"

struct Controller{
    Ann explorerAnn;
    float l = 0;
    float m = 0;
    float r = 0;
    float fitness=-1;
    float mean = 0;
    float stdev = 0;
    float var = 0;
};

struct Population{
    std::array<Controller,POP_SIZE> controller;
};


struct MeanStd{
    float mean = 0;
    float stdev = 0;
};
typedef Eigen::Vector3i Behavioral_Descriptor;

struct Obs{
    Behavioral_Descriptor bd;
    float actualFitness = 0;
};

struct DemoResult{
    Controller best_broken;
    Controller low_left;
    vector<Controller> candidates;
    Behavioral_Descriptor besfLowLeft_bd;
};

#endif //MATPLOTLIB_CPP_TYPES_H
