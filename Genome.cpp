//
// Created by robin on 12/05/2021.
//

#include <random>
#include <chrono>
#include <iostream>
#include "header/Genome.h"

Genome::Genome(float m0, float m1, float m2, float c0, float c1, float c2) : m0(m0), m1(m1), m2(m2), c0(c0), c1(c1),
                                                                           c2(c2) {}

Genome Genome::mutate(int fitness, int stagnationCount) {
    std::random_device rd;
    long unsigned int ns = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator{ns};
    std::uniform_int_distribution<> disParam(0,5);
    std::uniform_int_distribution<> disUpDown(0,1);

    Genome newGenome = Genome(m0, m1, m2, c0, c1, c2);
    for (int i = 0; i < std::min(stagnationCount,10); ++i) {

        float diff = disUpDown(generator)==1? -0.1:0.1;
        switch(disParam(generator)) {
            case 0 : newGenome.m0+=diff;
            break;
            case 1 : newGenome.m1+=diff;
            break;
            case 2 : newGenome.m2+=diff;
            break;
            case 3 : newGenome.c0+=diff;
            break;
            case 4 : newGenome.c1+=diff;
            break;
            default : newGenome.c2+=diff;
        }
    }
    return newGenome;
}

void Genome::print() {
    std::cout << "Genome: m0 " << std::to_string(m0) << " m1 " << std::to_string(m1)<< " m2 " <<  std::to_string(m2)<< " c0 " <<std::to_string(c0)<< " c1 " <<std::to_string(c1)<< " c2 " << std::to_string(c2) << std::endl;
}
