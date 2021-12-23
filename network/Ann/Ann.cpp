//
// Created by moger on 08.06.2021.
//

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <chrono>
#include <random>
#include "Ann.h"
using namespace std;




void Ann::printAnn(string name) {
    cout << "##########" << " " << name << " ANN   ########"<< endl;
    cout << "fitness: " << fitness << endl;
    for (auto layer:layers) {
        cout << "-------------Layer----------------" << endl;
        for(auto neuron:layer.neurons){
            cout << "-------------Neuron------------" << endl;
            for(Connection connection:neuron.connections) {
                cout << connection.input << " " << connection.weight << " " << endl;
            }
            cout << "Bias = " << neuron.bias << endl;
        }
    }
    cout << endl;
}
string Ann::toString() {
    // Create an output string stream
    std::ostringstream streamObj;

    streamObj << "layer neuron connection weight\n";
    int layerCount = 0;
    for (auto layer:layers) {
        int neuronCount = 0;
        for(auto neuron:layer.neurons){
            streamObj << " -999 -999 -999 " << neuron.bias << "\n";
            for(Connection connection:neuron.connections) {
                streamObj << layerCount << " " << neuronCount << " " << connection.input << " " << connection.weight << "\n";
            }

            neuronCount++;
        }
        layerCount++;
    }
    streamObj << "\n";
    return streamObj.str();
}
vector<Layer> Ann::readFromFile(string fileName) {
    ifstream dataFile(fileName);
    vector<Layer> layers;
    if(dataFile.is_open()) {

        int curLayer, curNeuron, prevNeuron = 0;
        float weight = 0;
        float bias = 0;
        string comment = " ";
        dataFile >> comment >> comment >> comment >> comment;
        while (dataFile >> curLayer >> curNeuron >> prevNeuron >> weight) {
            if(curLayer == -999 && curNeuron == -999 && prevNeuron == -999){
                bias = weight;
                continue;
            }
            if(layers.size() < curLayer+1) {
                layers.push_back(Layer());
            }
            if(layers[curLayer].neurons.size() < curNeuron+1){
                Neuron newN;
                newN.bias=bias;
                layers[curLayer].addNeuron(newN);
            }
            layers[curLayer].neurons[curNeuron].addConnection(prevNeuron, weight);
        }
    }else{
        cout << "Error opening the File! " + fileName + " " << endl;
        exit(1);
    }
    Ann(layers).printAnn(fileName);
    return layers;
}
float Ann::activationFunction(float input) {
    return 2/(1+exp(-2*input))-1;//0.2/(1+exp(-2*input));
}

vector<float> Ann::calcOutput(vector<float> input) {
    vector<float> result;

    for (auto &layer: layers){
        layer.resetNeuronValues();
    }
    //recursively calculate output starting from the last
    //layer
  /*  for(float f: oldOutput){
        input.push_back(f);
    }*/
    for (int i = 0; i < layers[layers.size()-1].neurons.size(); ++i) {
        result.push_back(transFunction(layers.size()-1,i, input));
    }
    oldOutput = result;
    return result;
}

float Ann::transFunction(int layer, int neuron, vector<float> input) {
    if(layer == -1){
        return input[neuron];
    }
    Neuron *curNeuron = &layers[layer].neurons[neuron];
    float value = curNeuron->value;
    if(value != INFINITY){
        return value;
    }
    float result = 0;
    vector<Connection> connections = curNeuron->connections;
    for(auto connection:connections){
        result += connection.weight* transFunction(layer-1,connection.input,input);
    }
    result += curNeuron->bias;
    result += curNeuron->prevWeight_*curNeuron->oldValue_;
    result = activationFunction(result);
    curNeuron->value = result;
    curNeuron->oldValue_ = result;
    return result;
}

Ann::Ann(const vector<Layer> layers) :layers(layers) {}



void Ann::mutate() {
    long unsigned int ns = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine g{ns};
    uniform_int_distribution<int> disType(0,10);
    int choice = disType(g);
    if(choice > 8){
        for(auto &layer:layers){
            mutateLayer(&layer, g, true);
        }
    }else if(choice > 7){
        for(auto &layer:layers){
            mutateLayer(&layer, g, false);
        }
    }else{
        uniform_int_distribution<int> disLayer(0,layers.size()-1);
        int layerIdx = disLayer(g);
        mutateLayer(&layers[layerIdx], g, false);
    }
}

void Ann::mutateLayer(Layer *layer, default_random_engine g, bool allNeurons){
    normal_distribution<float> disWiggle(0,0.1);

    if(allNeurons){
        for(auto &neuron : layer->neurons){
            neuron.bias +=disWiggle(g);
            neuron.prevWeight_+=disWiggle(g);
            for(auto &connection : neuron.connections) {
                connection.weight += disWiggle(g);
            }
        }
    }else{
        uniform_int_distribution<int> disNeuron(0,layer->neurons.size()-1);
        Neuron *curNeuron = &layer->neurons[disNeuron(g)];
        curNeuron->bias+=disWiggle(g);
        for(auto &connection:curNeuron->connections){
            connection.weight += disWiggle(g);
        }
    }
}

void Ann::init() {
    long unsigned int ns = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine g{ns};
    uniform_real_distribution<float> disInterval(-0.1,0.1);
    uniform_real_distribution<float> disIntervalBias(0,0.05);
    for (auto &layer : layers) {
        for(auto &neuron : layer.neurons){
            neuron.bias = disIntervalBias(g);
            for(auto &connection : neuron.connections) {
                connection.weight = disInterval(g);
            }
        }
    }

}


vector<Layer> Ann::createStructure(string fileName) {
    ifstream dataFile(fileName);
    vector<Layer> layers;
    if(dataFile.is_open()) {

        int curLayer, curNeuron, prevNeuron = 0;
        float weight = 0;
        string comment = " ";
        dataFile >> comment >> comment >> comment >> comment;
        while (dataFile >> curLayer >> curNeuron >> prevNeuron >> weight) {
            if(layers.size() < curLayer+1) {
                layers.push_back(Layer());
            }
            if(layers[curLayer].neurons.size() < curNeuron+1){
                Neuron newN;
                layers[curLayer].addNeuron(newN);
            }
            layers[curLayer].neurons[curNeuron].addConnection(prevNeuron, weight);
        }
    }else{
        cout << "Error opening the File!" << endl;
        exit(1);
    }
    //Ann(layers).printAnn(fileName);
    return layers;
}

Ann::Ann() {}

