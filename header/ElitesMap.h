//
// Created by r on 24.11.21.
//

#ifndef MATPLOTLIB_CPP_ELITESMAP_H
#define MATPLOTLIB_CPP_ELITESMAP_H
#include "types.h"
#include "array"

class ElitesMap{
public:
    vector<Obs> obs;

    std::vector<Behavioral_Descriptor> occupied_niches;
    Controller bestController;
    int cardinality = 0;
    Controller bestObservedController;
    std::array<std::array<std::array<Controller, DESCIRPTOR_DIM>, DESCIRPTOR_DIM>, DESCIRPTOR_DIM> elites;
public:
    Controller getControllerByBd(Behavioral_Descriptor bd);
    void setControllerBybd(Controller con, Behavioral_Descriptor bd);
};


#endif //MATPLOTLIB_CPP_ELITESMAP_H
