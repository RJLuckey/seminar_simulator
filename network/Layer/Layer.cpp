//
// Created by moger on 08.06.2021.
//

#include "Layer.h"

Layer::Layer(const vector<Neuron> &neurons) : neurons(neurons) {}

Layer::Layer(){}

Layer::Layer(float bias) : bias(bias) {}

void Layer::addNeuron(Neuron neuron) {
    neurons.push_back(neuron);
}

void Layer::resetNeuronValues(){
    for(auto &neuron: neurons){
        neuron.value = INFINITY;
    }
}
