//
// Created by robin on 29/04/2021.
//

#ifndef EVO_WORLD_H
#define EVO_WORLD_H


#include <vector>
#include <opencv2/core/types.hpp>
#include "Sensor.h"
#include "Controllable.h"
#include "Placeable.h"
#include "Collidable.h"

class World {
private:
    std::vector<Controllable*> controllables;
    std::vector<Placeable*> placeables;
public:

    const std::vector<Controllable *> &getControllables() const;

    const std::vector<Placeable *> &getPlaceables() const;

    const std::vector<Collidable *> &getCollidables() const;

private:
    std::vector<Collidable*> collidables;
    //TODO add update Methods
public:
    void addControllable(Controllable* c);
    void addPlaceable(Placeable* c);
    void addCollidable(Collidable* c);
};


#endif //EVO_WORLD_H
