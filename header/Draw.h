//
// Created by robin on 02/05/2021.
//

#ifndef EVO_DRAW_H
#define EVO_DRAW_H
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "World.h"
#include "../network/Ann/Ann.h"
#include "../constants.h"
#include "types.h"
#include "ElitesMap.h"
class Draw {
public:
    static void drawWorld(World *w, bool writeToFile, float fitness, int timestep, int gen);
    void drawHeatmap(ElitesMap elMap, int gen, int counter);
    cv::Mat drawLightMap(std::vector<Placeable*> p);
    void drawPop(std::array<Controller, POP_SIZE>pop);

};


#endif //EVO_DRAW_H
