//
// Created by r on 17.11.21.
//

#include "header/Mboa.h"
#include <iostream>
#include "constants.h"

Eigen::MatrixXf gp_kernel(std::vector<Behavioral_Descriptor> x, std::vector<Behavioral_Descriptor> x_p) {
    float sizeX = x.size();
    float sizeX_p = x_p.size();
    Eigen::MatrixXf C;
    C.resize(sizeX,sizeX_p);
    C.setZero();
    for (int i = 0; i < sizeX; ++i) {
        for (int j = 0; j < sizeX_p; ++j) {
            float dist = exp(-(pow((x[i]-x_p[j]).norm(),2)));
            //cout << dist << endl;
            C(i,j) = dist;
        }
    }
    return C;
}




ElitesMap Mboa::predict(ElitesMap elMap, Controller con, Behavioral_Descriptor bd) {

    Eigen::Matrix<float, 10, 10> V;
    V.setOnes();



    vector<float> performanceDiff;

    for(auto obs : elMap.obs){
        performanceDiff.push_back(obs.actualFitness-elMap.getControllerByBd(obs.bd).fitness);
    }

    Eigen::MatrixXf diffMatrix;
    diffMatrix = Eigen::MatrixXf::Map(&performanceDiff[0],  performanceDiff.size(),1);

    vector<Behavioral_Descriptor> x_p;
    x_p.push_back(bd);
    Eigen::MatrixXf k_a;
    k_a.resize(performanceDiff.size(), 1);
    k_a = gp_kernel(elMap.occupied_niches, x_p);
   // std::cout << k_a << endl;
    Eigen::MatrixXf k_a_t;
    k_a_t.resize(performanceDiff.size(),performanceDiff.size());
    k_a_t = k_a.transpose();
    Eigen::MatrixXf k;
    k.resize(performanceDiff.size(),performanceDiff.size());
    k = gp_kernel(elMap.occupied_niches, elMap.occupied_niches);
    Eigen::MatrixXf k_inv = k.inverse();
    Eigen::MatrixXf k_aa = gp_kernel(x_p, x_p);

    con.fitness = con.fitness + ((k_a_t*k_inv).transpose()*(diffMatrix)).norm();
    con.var = (k_aa-(k_a_t*k_inv*k_a)).norm();
    elMap.setControllerBybd(con, bd);
    return elMap;
}


void cov(){

}
Behavioral_Descriptor Mboa::acquire(ElitesMap elMap) {
    float maxFitness = 0;
    Behavioral_Descriptor best;
    for (Behavioral_Descriptor & bd: elMap.occupied_niches) {
        Controller con = elMap.getControllerByBd(bd);
        if(con.fitness + con.stdev*0.5 > maxFitness){
            best = bd;
        }
    }
    return best;
}

float Mboa::evaluate(Controller c) {

    return 0.5;
}
