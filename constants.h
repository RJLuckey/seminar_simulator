//
// Created by robin on 17/06/2021.
//




#ifndef MATPLOTLIB_CPP_CONSTANTS_H
#define MATPLOTLIB_CPP_CONSTANTS_H
#include <string>
//legacy
const int cellsDim = 100;
const double chargerRadius = 0.2;
const float goalX = 0.5;
const float goalY = 1;

//Meta params
const int NUMBER_OF_GENERATIONS = 20000;
const int POP_SIZE = 24;
const int TIMESTEPS = 200;
const int THREADS = 24;
const int REPS=100;
const float NOICE = 0.0025;
const int DESCIRPTOR_DIM = 30;
const int TEST_FREQ = 500;
const int LEFT_BROKEN = 0;
const int RIGHT_BROKEN = 2;


//drawing and plotting
const int DRAW_FREQ = 100;
const int TO_FILE_FREQ = 1000;
const int CONSOLE_FREQ = 10000;
const int PLOT_FREQ = 20;
const int RESULT_FREQ = 100;
const int SAVE_ANN_FREQ = 500;
const int DRAW_SPEED = 1;
const int HEATMAP_SCALE = 50;
//demo

const bool DEMO_ONLY = false;
const int TRAINED_NET = 5000;
#endif //MATPLOTLIB_CPP_CONSTANTS_H
