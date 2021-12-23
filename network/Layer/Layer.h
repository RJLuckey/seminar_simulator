//
// Created by moger on 08.06.2021.
//

#ifndef EVOSHEET4TASK1_LAYER_H
#define EVOSHEET4TASK1_LAYER_H

#include <vector>
#include "../Neuron/Neuron.h"

using namespace std;

class Layer {
public:

    float bias;
    vector<Neuron> neurons;

    Layer();

    Layer(const vector<Neuron> &neurons);

    Layer(float bias);

    void addNeuron(Neuron neuron);
    void resetNeuronValues();



};


#endif //EVOSHEET4TASK1_LAYER_H
