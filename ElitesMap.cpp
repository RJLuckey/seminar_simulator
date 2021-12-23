//
// Created by r on 24.11.21.
//

#include "header/ElitesMap.h"

Controller ElitesMap::getControllerByBd(Behavioral_Descriptor bd) {

    return elites[bd[0]][bd[1]][bd[2]];
}

void ElitesMap::setControllerBybd(Controller con, Behavioral_Descriptor bd) {
    elites[bd[0]][bd[1]][bd[2]] = con;
}
