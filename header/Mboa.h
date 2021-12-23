//
// Created by r on 17.11.21.
//

#ifndef MATPLOTLIB_CPP_MBOA_H
#define MATPLOTLIB_CPP_MBOA_H
#include "Eigen/Core"
#include "Eigen/LU"
#include "types.h"
#include "ElitesMap.h"
class Mboa {
public:
    Behavioral_Descriptor acquire(ElitesMap elMap);

    ElitesMap predict(ElitesMap elMap, Controller con, Behavioral_Descriptor bd);
    float evaluate(Controller c);
};


#endif //MATPLOTLIB_CPP_MBOA_H
