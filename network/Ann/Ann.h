//
// Created by moger on 08.06.2021.
//

#ifndef EVOSHEET4TASK1_ANN_H
#define EVOSHEET4TASK1_ANN_H
#include <vector>
#include "../Layer/Layer.h"
#include "../Connection/Connection.h"
#include <random>
class Ann {

public:
    vector<Layer> layers;
    float fitness=0;
    Ann();
    Ann(const vector<Layer> layers);
    string toString();
    void printAnn(string name);
    float activationFunction(float input);
    vector<float> calcOutput(vector<float> input);
    float transFunction(int layer, int neuron, vector<float> input);
    void mutateLayer(Layer *layer, default_random_engine g, bool allNeurons);
    void mutate();
    void init();
    vector<float> oldOutput = {0,0};
    static vector<Layer> readFromFile(string fileName);


    static vector<Layer> createStructure(string fileName);
};


#endif //EVOSHEET4TASK1_ANN_H
