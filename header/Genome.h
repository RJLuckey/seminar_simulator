//
// Created by robin on 12/05/2021.
//

#ifndef EVO_GENOME_H
#define EVO_GENOME_H


class Genome {
public:
    float m0;
    float m1;
    float m2;
    float c0;
    float c1;
    float c2;
public:
    Genome(float m0, float m1, float m2, float c0, float c1, float c2);

    void print();

    Genome mutate(int fitness, int stagnationCount);
};


#endif //EVO_GENOME_H
