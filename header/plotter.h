//
// Created by r on 11.11.21.
//
//#include "../matplotplusplus/source/matplot/matplot.h"
//#include "matplotlibcpp.h"
#include <matplot/matplot.h>
#include "types.h"
#include "../constants.h"
using namespace matplot;

#ifndef MATPLOTLIB_CPP_PLOTTER_H
#define MATPLOTLIB_CPP_PLOTTER_H
class Plotter{
public:
    Plotter();

    figure_handle f = figure(false);
    axes_handle axes;
    void plotDemoResults(vector<int> plot_gens,
    vector<float> bests, vector<float> stdevs, vector<float> avgs,
            vector<float> all, vector<float> all_std, vector<float> none, vector<float> none_std,
            vector<float> left, vector<float> left_std,vector<float> right, vector<float> right_std,
            vector<Behavioral_Descriptor> bestControllers, vector<Behavioral_Descriptor> adaptedControllers,
            int counter
    );
};


#endif //MATPLOTLIB_CPP_PLOTTER_H
