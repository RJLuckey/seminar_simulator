//
// Created by r on 01.12.21.
//
#include "header/plotter.h"

void Plotter::plotDemoResults(const vector<int> plot_gens,
                              const vector<float> bests, const vector<float> stdevs, const vector<float> avgs,
                              const vector<float> best_fitness_broken, const vector<float> best_fitness_broken_std,
                              const vector<float> left, const vector<float> left_std,
                              const vector<float> sensorusageBestLeft, const vector<float> sensorusageBestRight,
                              const vector<float> sensorusageLeft, const vector<float> sensorusageRight,
                              vector<Behavioral_Descriptor> bestControllers, vector<Behavioral_Descriptor> adaptedControllers,
                              int counter
){



    auto sub1 = subplot(2, 3, 5);
    errorbar(sub1,plot_gens, bests, stdevs);
    xlabel("Generations");
    ylabel("Fitness");
    title("Best fitness");

    auto sub2 = subplot(2, 3, 2);
    plot(sub2,plot_gens, avgs);
    xlabel("Generations");
    ylabel("Fitness");
    title("Average Fitness");


    auto sub3 = subplot(2, 3, 3);
    errorbar(sub3,plot_gens, best_fitness_broken, best_fitness_broken_std);
    xlabel("Generations");
    ylabel("Fitness");
    title("Orig controller with broken left Sensor");

    auto sub4 = subplot(2, 3, 4);
    errorbar(sub4, plot_gens, left, left_std);
    xlabel("Generations");
    ylabel("Fitness");
    title("Fitness, adapted controller");

    auto sub5 = subplot(2, 3, 1);
    sub5->hold(true);
    plot(sub5, plot_gens, sensorusageBestLeft)->display_name("Orig Controller");
    plot(sub5, plot_gens, sensorusageLeft)->display_name("Adapted Controller");
    sub5->hold(false);
    xlabel("Generations");
    ylabel("Average sensor usage");
    sub5->legend();
    title("Average sensor usage left");


    auto sub6 = subplot(2, 3, 6);
    sub6->hold(true);
    plot(sub6,plot_gens, sensorusageBestRight)->display_name("Orig Controller");
    plot(sub6,plot_gens, sensorusageRight)->display_name("Adapted Controller");
    sub6->hold(false);
    xlabel("Generations");
    ylabel("average sensor usage");
    sub5->legend();
    title("Average sensor usage right");
    sgtitle("Performance Comparison");

    matplot::legend(f->current_axes(),true);
    f->size(2000,1000);

    //f->draw();
    f->save("../results/"+to_string(DESCIRPTOR_DIM)+"_"+to_string(counter)+".png");

}

Plotter::Plotter() {
    f->size(2000,1000);
    //axes = f->current_axes();
   // axes->hold(true);
}

void plotBestAVG(vector<float> bests, vector<float> stdevs, vector<float> avgs){
    vector<float>ts;
    for (int i = 0; i < bests.size(); ++i) {
        ts.push_back(i+1);

    }

    cla();




    subplot(1, 2, 1);
    auto eb = errorbar(ts, bests, stdevs);//->filled_curve(true);
    xlabel("Generations");
    ylabel("Fitness");
    eb->display_name("Fitness");
    subplot(1, 2, 2);
    auto p1 = plot(avgs);
    xlabel("Generations");
    ylabel("Fitness");
    title("Sensor Problem");
    p1->display_name("average fitness ");
    sgtitle("Sensor Problem");
    /*plt::subplot(1, 3, 3);
        plt::scatter(ts, avgs);
        plt::ylabel("Fitness");
        plt::xlabel("ANNS");
        plt::ylim(0,1);*/



}