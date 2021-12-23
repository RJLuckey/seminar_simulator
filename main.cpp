

#include "constants.h"
#include <iostream>
#include "header/World.h"
#include "header/Draw.h"
#include "header/ProxVehicle.h"
#include "header/Wall.h"
#include <future>
#include <random>
#include "network/Ann/Ann.h"
#include <fstream>
#include "header/plotter.h"
#include "header/types.h"
#include "header/Mboa.h"
#include <chrono>
const vector<Layer> annTemplateExplorer = Ann::createStructure("../structureExplorer");


float calcMean(vector<float> v){
    float sum = std::accumulate(v.begin(), v.end(), 0.0);
    return sum / (v.size());
}
float calcMeanPop(std::array<Controller, POP_SIZE> p){

    float sum = std::accumulate(p.begin(), p.end(), 0.0,[](const float& a, Controller& b)
    {
        return a + b.fitness;
    });
    return sum / (p.size());
}
MeanStd stdev(vector<float> v){
    MeanStd result;

    float mean = calcMean(v);

    std::vector<float> diff(v.size());
    std::transform(v.begin(), v.end(), diff.begin(), [mean](float x) { return x - mean; });
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / v.size());
    result.mean = mean;
    result.stdev = stdev;
    return result;
}

Controller setResults(float l, float m, float r, int timestep, float fitness){
    Controller team;
    if(!timestep){timestep = 1;}
    team.l = l / timestep;
    team.m = m / timestep;
    team.r = r / timestep;
    team.fitness= fitness;
    return team;
}
Controller  runSim(World w, int  timesteps, bool draw, bool writeToFile, int gen, int mode = -1, int randomoffset = 0){


    Controller team;
    ProxVehicle* exv = (ProxVehicle*)w.getControllables().at(0);
    float l = 0;
    float m = 0;
    float r = 0;
    float v = 0;
    int i=0;
    bool crash = false;
    float x_old = 0;
    float y_old = 0;
    float fitness = 0;
    do{
        i++;
        if(exv->center_y_>=0.95){
            exv->center_y_=0.0;
        }
        if(exv->center_y_<0){
            exv->center_y_=1;
        }
        x_old = exv->center_x_;
        y_old = exv->center_y_;

        crash = exv->control(mode, randomoffset);

        fitness+= exv->center_y_ - y_old;

        l += exv->plv_track;
        m += exv->pmv_;
        r += exv->prv_track;

        if (draw && i % DRAW_SPEED == 0) {
            Draw d;
            d.drawWorld(&w, false, fitness, i, gen);
        }
        if(fitness<0)crash=true;
        if(draw)crash=false;
    }while(!crash && i < timesteps);


    if(writeToFile){
        Draw d;
        d.drawWorld(&w, writeToFile, fitness, i, gen);
    }

    return setResults(l,m,r,i,fitness);

}

Controller runWithNewVehicle(World w, Controller con, bool draw, bool writeToFile, int gen, int mode = -1) {
    ProxSensor pl = ProxSensor(w.getCollidables());
    ProxSensor pm = ProxSensor(w.getCollidables());
    ProxSensor pr = ProxSensor(w.getCollidables());



    ProxVehicle pv = ProxVehicle(0.5, 0.1, 0.055, 0.02, 90, pl, pm, pr, "prox1", con);
    pv.xOffset_=pv.width_/2;
    pv.setAllCollidables(w.getCollidables());
    w.addControllable((Controllable*)&pv);
    con.l = 0;
    con.m = 0;
    con.r = 0;
    con.fitness = 0;
    vector<float> v;
    for (int i = 0; i < REPS; ++i) {
        if(i>0){
            draw = false;
        }
        Controller temp = runSim(w, TIMESTEPS, draw, writeToFile, gen, mode,i);
        con.l += temp.l;
        con.m += temp.m;
        con.r += temp.r;
        con.fitness += temp.fitness;
        v.push_back(temp.fitness);
    }
    MeanStd mstd = stdev(v);
    con.stdev = mstd.stdev;
    con.fitness = mstd.mean;
    con.l /= REPS;
    con.m /= REPS;
    con.r /= REPS;
    return con;

}



void sortPop(std::array<Controller,POP_SIZE>* pop){
    sort(pop->begin( ), pop->end( ), [ ](const auto& lhs, const auto& rhs )
    {
        return lhs.fitness > rhs.fitness;
    });
}
Population createAgents(int size, const vector<Layer> explorer, bool init){

    Population pop;
    for (int i = 0; i < size; ++i) {
        pop.controller[i].explorerAnn=Ann(explorer);
        if(init){
            pop.controller[i].explorerAnn.init();
        }

    }
    return pop;
}


Population runParallel(Population &pop, World w, int gen, bool runOnce){

    if(runOnce){
        pop.controller[0] = runWithNewVehicle(w, pop.controller[0], 1, 1,gen);
    }else{

        bool writeToFile = false;
        bool draw = false;


        array<future<Controller>,THREADS> results;

        int subpopsize = POP_SIZE / THREADS;
        for (int n = 0; n < subpopsize; ++n) {
            for (int i = 0; i < THREADS; ++i) {
                if(n==0 && i==0){
                    if(gen%TO_FILE_FREQ==0){

                        writeToFile = true;
                    }

                }

                results[i] = async( launch::async, runWithNewVehicle, w, pop.controller[n * THREADS +i], draw, writeToFile,gen,-1);
                writeToFile = false;
                draw = false;
            }
            for (int j = 0; j < THREADS; ++j) {
                pop.controller[n * THREADS + j] = results[j].get();

            }
        }
    }
    return pop;
}

std::vector<Collidable>  createWalls(std::vector<Collidable> collidables){


    Wall tunnel_left = Wall(0.42, 0.5,0.02,1,0,"tleft");
    Wall tunnel_right = Wall(0.58,0.5,0.02,1,0,"tright");

    collidables.push_back(tunnel_left);
    collidables.push_back(tunnel_right);

    return collidables;

}
void sortSlice(vector<Controller> *slice){
    sort(slice->begin( ), slice->end( ), [ ](const auto& lhs, const auto& rhs )
    {
        return lhs.fitness > rhs.fitness;
    });
}
vector<Controller> findBestWith(ElitesMap elMap, float mode){
    vector<Controller> bests;


    if(mode == LEFT_BROKEN){
        for (int i = 0; i < DESCIRPTOR_DIM; ++i) {
            for (int j = 0; j < DESCIRPTOR_DIM; ++j) {
                //if(elMap.elites[0][i][j].fitness > bests[bests.size()-1].fitness){
                bests.push_back(elMap.elites[0][i][j]);
                //}

            }
        }
    }
    sortSlice(&bests);
    return bests;
}
Controller simXTiems(Controller con, World w, int mode=-1, bool draw = false){
    Controller result;
    result = runWithNewVehicle(w, con,  draw, true, 0, mode);
    cv::waitKey(20);

    return result;
}
Behavioral_Descriptor getDescriptor(Controller t){
    Behavioral_Descriptor bd;
    int l = (int) floor(t.l*DESCIRPTOR_DIM);
    int m = (int) floor(t.m*DESCIRPTOR_DIM);
    int r = (int) floor(t.r*DESCIRPTOR_DIM);
    bd[0] = l;
    bd[1] = m;
    bd[2] = r;
    return bd;
}
DemoResult  demo(ElitesMap elMap, float loadFromFile = false){

    World w = World();
    DemoResult result;

    std::vector<Collidable> collidables;
    collidables = createWalls(collidables);

    for(auto &c : collidables){
        w.addCollidable(&c);
    }
    Controller con;
    if(loadFromFile){
        const vector<Layer> genomeExplorer = Ann::readFromFile("../"+to_string(TRAINED_NET)+".txt");
        Ann ann = Ann(genomeExplorer);
        con.explorerAnn = ann;
        simXTiems(con, w, -1,true);
    }
    else{
        //demo best
        //simXTiems(reps, con, w);

        //demo others
        if(elMap.occupied_niches.size() > 0){
           /* Controller low_right = findBestWith(elMap, 0);
            std::cout << "low right with fitness " << low_right.fitness;
            std::cout << "l " << elMap.bestController.l << " m " << elMap.bestController.m << " r " << elMap.bestController.r << endl;
            con = low_right;
            if(low_right.fitness!=-1){
                cout << "All Working:" << endl;
                Controller result = simXTiems(200, con, w);
                cout << "Fitness with working sensor " << result.fitness << " stdev " << result.stdev << endl;
                result = simXTiems(200, con, w, RIGHT_BROKEN);
                cout << "Fitness with broken right sensor " << result.fitness << " stdev " << result.stdev <<  endl;
                result = simXTiems(200, con, w, LEFT_BROKEN);
                cout << "Fitness with broken left sensor " << result.fitness << " stdev " << result.stdev <<  endl;
                result = simXTiems(200, con, w, 3);
                cout << "Fitness with all sensors broken " << result.fitness << " stdev " << result.stdev <<  endl;

            }*/




            vector<Controller> leftists = findBestWith(elMap, LEFT_BROKEN);


            if(leftists[0].fitness!=-1){
                Controller bestLowLeft;
                cout << "Controllers choosen by Sensor usage and fitness " << endl;
                for (int i = 0; i < 5; ++i) {
                    Controller low_left_candidate = leftists[i];
                    result.candidates.push_back(leftists[i]);
                    Controller low_left = simXTiems(low_left_candidate, w, LEFT_BROKEN);
                    cout << "Controller " << i << " elMap fitness: " << low_left_candidate.fitness << " stdev: " << low_left_candidate.stdev <<  " real fitness: " << low_left.fitness << endl;
                     if(low_left.fitness > bestLowLeft.fitness){
                         bestLowLeft = low_left;
                         result.besfLowLeft_bd = getDescriptor(low_left_candidate);
                     }
                }

                result.low_left = bestLowLeft;
                cout << "Usiing Adapted Controller: " + result.besfLowLeft_bd[0] << " " << result.besfLowLeft_bd[1] << " " << result.besfLowLeft_bd[2] << endl;
                Controller best = elMap.bestController;
                result.best_broken = simXTiems(best, w, LEFT_BROKEN);
                cout << "Controller with highest fitness elMap fitness:" << best.fitness << " stdev: " << best.stdev << " real fitness: " << result.best_broken.fitness << " stdev " << result.best_broken.stdev << endl;
            }
        }
    }
   return result;
}



void printMapToFile(const int gen, const ElitesMap elMap, int counter){
    std::fstream fs;
    string filename = "../elmaps/"+to_string(DESCIRPTOR_DIM)+"_"+to_string(counter)+".txt";
    fs.open (filename, std::fstream::out | std::fstream::app);
    std::ostringstream streamObj;
    Behavioral_Descriptor  bestBd = getDescriptor(elMap.bestController);
    Behavioral_Descriptor  bestObsBd = getDescriptor(elMap.bestObservedController);
    streamObj << gen << " "<< elMap.cardinality << " " << elMap.bestController.fitness << " " <<  bestBd[0] << " " << bestBd[1] << " " << bestBd[2] << " " << elMap.bestObservedController.fitness << " " <<  bestObsBd[0] << " " << bestObsBd[1] << " " << bestObsBd[2] << endl;
    for (int i = 0; i < DESCIRPTOR_DIM; ++i) {
        for (int j = 0; j < DESCIRPTOR_DIM; ++j) {
            streamObj << i << " " << j;
            for (int k = 0; k < DESCIRPTOR_DIM; ++k) {
                streamObj << " " << elMap.elites[i][j][k].fitness;
            }
            streamObj << endl;
        }
    }
    fs << streamObj.str()  << endl;
    fs.close();
}
void printDataToFile(const vector<int> plot_gens,
                const vector<float> bests, const vector<float> stdevs, const vector<float> avgs,
                const vector<float> best_fitness_broken, const vector<float> best_fitness_broken_std,
                const vector<float> left, const vector<float> left_std,
                const vector<float> sensorusageBestLeft, const vector<float> sensorusageBestRight,
                const vector<float> sensorusageLeft, const vector<float> sensorusageRight,
                const vector<Behavioral_Descriptor> bestControllers, const vector<Behavioral_Descriptor> adaptedControllers,
                const vector<vector<Controller>> candidates,
                int counter){


    std::ostringstream candidates_s;
    for (auto c : candidates.back()) {
        Behavioral_Descriptor cbd = getDescriptor(c);
        candidates_s << " " << cbd[0] << " " <<cbd[1] << " " << cbd[2] << " " << c.fitness << " " << c.stdev << " ";
    }
    std::fstream fs;
    string filename = "../results/data_"+to_string(DESCIRPTOR_DIM)+"_"+to_string(counter)+".txt";
    fs.open (filename, std::fstream::out | std::fstream::app);
    std::ostringstream streamObj;
    streamObj << plot_gens.back() << " "    //0
    << bestControllers.back()[0] << " "     //1
    << bestControllers.back()[1]<< " "      //2
    << bestControllers.back()[2]<< " "      //3
    << adaptedControllers.back()[0] << " "  //4
    << adaptedControllers.back()[1]<< " "   //5
    << adaptedControllers.back()[2] << " "  //6
    << bests.back()<< " "                   //7
    << stdevs.back() << " "                 //8
    <<avgs.back() << " "                    //9
    <<best_fitness_broken.back()<< " "      //10
    <<best_fitness_broken_std.back()<< " "  //11
    <<left.back()<< " "                     //12
    <<left_std.back()<<" "                  //13
    <<sensorusageBestLeft.back()<<" "       //14
    <<sensorusageBestRight.back()<<" "      //15
    <<sensorusageLeft.back()<<" "           //16
    <<sensorusageRight.back();              //17

    fs << streamObj.str() << candidates_s.str() << endl;
    fs.close();
}


int main() {

    if(DEMO_ONLY){
        demo(ElitesMap(), true);
        return 0;
    }
    for (int t = 0; t < 20; ++t) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();


        World w = World();
        std::vector<Collidable> collidables;
        collidables = createWalls(collidables);

        for(auto &c : collidables){
            w.addCollidable(&c);
        }


        long unsigned int ns = chrono::system_clock::now().time_since_epoch().count();
        default_random_engine g{ns};

        ElitesMap elMap;
        elMap.occupied_niches.reserve(DESCIRPTOR_DIM*DESCIRPTOR_DIM*DESCIRPTOR_DIM);
        vector<float> bests;
        vector<float> stdevs;
        vector<float> avgs;
        //--demos

        vector<float> left;
        vector<float> left_std;
        vector<float> sensorusageLeft;
        vector<float> sensorusageRight;
        vector<float> best_fitness_broken;
        vector<float> best_fitness_broken_std;
        vector<float> sensorusageBestLeft;
        vector<float> sensorusageBestRight;
        vector<vector<Controller>> candidates;
        vector<Behavioral_Descriptor> bestControllers;
        vector<Behavioral_Descriptor> adaptedControllers;
        float sum_usage_best_r = 0;
        float sum_usage_best_l = 0;
        float sum_usage_left_r = 0;
        float sum_usage_left_l = 0;
        vector<int> plot_gens;
        Plotter p;

        Mboa mboa;
        Population pop = createAgents(POP_SIZE, annTemplateExplorer, true);
        int gen = 0;
        while(gen <= NUMBER_OF_GENERATIONS) {
          //  cout << "new Gen " << gen<< endl;
            pop = runParallel(pop, w, gen, false);
          //  cout << "end of Gen " << gen << endl;
            for (int i = 0; i < POP_SIZE; ++i) {
                if(pop.controller[i].fitness > elMap.bestController.fitness){
                    elMap.bestController = pop.controller[i];
                }
                Behavioral_Descriptor bd = getDescriptor(pop.controller[i]);
                Controller con = elMap.getControllerByBd(bd);
                if(con.fitness == -1){
                    elMap.cardinality++;
                    elMap.occupied_niches.push_back(bd);
                }
                if(con.fitness < pop.controller[i].fitness ){
                    con = pop.controller[i];
                    elMap.setControllerBybd(con, bd);
                }
            }
            if(gen%5000==0){
                printMapToFile(gen, elMap, t);
            }


    /*        if(gen>0 && gen%SAVE_ANN_FREQ == 0){
                std::fstream fs;
                fs.open ("../" + to_string(gen) +".txt", std::fstream::out);

                fs << elMap.bestController.explorerAnn.toString();

                fs.close();
            }*/

            if(gen == NUMBER_OF_GENERATIONS) {
                int count = 1;
                std::cout << "Best fitness " << elMap.bestController.fitness << endl;
                std::cout << "Cardinality " << elMap.cardinality << endl;
                std::cout << "Generation " << gen << " ";
                for (auto &controller:pop.controller) {
                    std::cout << " # " << controller.fitness << " stdev: " << controller.stdev;
                    if (count % POP_SIZE == 0) {
                        std::cout << std::endl;
                    }
                    count++;
                }
           // }
           // if(gen!=0 && gen % PLOT_FREQ == 0){


                Draw d;
                d.drawHeatmap(elMap, gen, t);





                if (plot_gens.size() > 1) {
                    p.plotDemoResults(plot_gens,
                                      bests, stdevs, avgs,
                                      best_fitness_broken, best_fitness_broken_std,
                                      left, left_std,
                                      sensorusageBestLeft, sensorusageBestRight,
                                      sensorusageLeft, sensorusageRight,  bestControllers,  adaptedControllers, t);

                }

            }
            if(gen != 0 && gen % TEST_FREQ == 0) {
                cout << "Generation: "<<to_string(gen)<<endl;

                int itCount = 0;


                DemoResult result = demo(elMap);
                if (result.low_left.fitness != -1) {
                    float avg = 0;
                    for (auto& bd: elMap.occupied_niches) {
                        avg += elMap.getControllerByBd(bd).fitness;
                    }
                    avgs.push_back(avg/elMap.cardinality);

                    sum_usage_best_l += elMap.bestController.l;
                    sum_usage_best_r += elMap.bestController.r;

                    sum_usage_left_l += result.low_left.l;
                    sum_usage_left_r += result.low_left.r;

                    int count = avgs.size();
                    sensorusageBestLeft.push_back(elMap.bestController.l);
                    sensorusageBestRight.push_back(elMap.bestController.r);
                    sensorusageLeft.push_back(result.low_left.l);
                    sensorusageRight.push_back(result.low_left.r);



                    plot_gens.push_back(gen);
                    bests.push_back(elMap.bestController.fitness);
                    stdevs.push_back(elMap.bestController.stdev);

                    best_fitness_broken.push_back(result.best_broken.fitness);
                    best_fitness_broken_std.push_back(result.best_broken.stdev);

                    left.push_back(result.low_left.fitness);
                    left_std.push_back(result.low_left.stdev);

                    bestControllers.push_back(getDescriptor(elMap.bestController));
                    adaptedControllers.push_back(result.besfLowLeft_bd);

                    candidates.push_back(result.candidates);

                    printDataToFile(plot_gens,
                                    bests, stdevs, avgs,
                                    best_fitness_broken, best_fitness_broken_std,
                                    left, left_std,
                                    sensorusageBestLeft, sensorusageBestRight,
                                    sensorusageLeft, sensorusageRight,
                                    bestControllers,  adaptedControllers,
                                    candidates,
                                    t);

                }



            }
            if(elMap.cardinality > DESCIRPTOR_DIM){
                Population newPop;
                for (int i = 0; i < POP_SIZE; ++i) {
                    std::uniform_int_distribution<> dis(0,elMap.occupied_niches.size()-1);
                    Behavioral_Descriptor bd = elMap.occupied_niches[dis(g)];
                    Controller modCon;
                    Ann modAnn = elMap.elites[bd[0]][bd[1]][bd[2]].explorerAnn;
                    modAnn.mutate();
                    modCon.explorerAnn = modAnn;
                    newPop.controller[i] = modCon;
                }
                pop = newPop;
            }else{
                pop = createAgents(POP_SIZE, annTemplateExplorer, true);
            }

            gen++;
        }

            std::fstream fs;
            string filename = "../" + to_string(TRAINED_NET) +".txt";
            fs.open (filename, std::fstream::out);

            fs << elMap.bestController.explorerAnn.toString();
            fs.close();
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        std::cout << "Run took = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
    }
    return 0;
}


