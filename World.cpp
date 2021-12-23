//
// Created by robin on 29/04/2021.
//

#include "header/World.h"

void World::addControllable(Controllable* c) {
    controllables.push_back(c);
}

void World::addPlaceable(Placeable* p) {
    placeables.push_back(p);
}

void World::addCollidable(Collidable* c) {
    collidables.push_back(c);
}

const std::vector<Controllable *> &World::getControllables() const {
    return controllables;
}

const std::vector<Placeable *> &World::getPlaceables() const {
    return placeables;
}

const std::vector<Collidable *> &World::getCollidables() const {
    return collidables;
}
