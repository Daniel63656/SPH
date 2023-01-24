#pragma once

#include <vector>
#include "datastructures/grid.h"

template<typename V>
class Simulation
{
public:
    void step(double dt);

private:
    std::vector<Particle<V>> particles;
    Grid<V> grid;

    void update();
};
