//
// Created by moger on 08.06.2021.
//

#ifndef EVOSHEET4TASK1_NEURON_H
#define EVOSHEET4TASK1_NEURON_H


#include "../Connection/Connection.h"
#include <vector>
#include <cmath>
using namespace std;

class Neuron {
public:
    Neuron(float value, const vector<Connection> &connections);
    Neuron();
    void addConnection(int input, float weight);

    float value = INFINITY;
    vector<Connection> connections;
    float bias = 0;
    float prevWeight_ =0;
    float oldValue_ =0;

};


#endif //EVOSHEET4TASK1_NEURON_H
