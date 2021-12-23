//
// Created by moger on 08.06.2021.
//

#include "Neuron.h"


Neuron::Neuron(float value, const vector<Connection> &connections) : value(value), connections(connections) {}

Neuron::Neuron(){
    value = INFINITY;
    bias = 0;
}

void Neuron::addConnection(int input, float weight){
    connections.push_back(Connection(input, weight));
}
