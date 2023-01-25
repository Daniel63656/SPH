#pragma once

#include <vector>
#include "../datastructures/grid.h"

template<unsigned int N>
class Simulation
{
public:
    void step(double dt);

private:
    std::vector<Particle<N>> particles;
    Grid<N> grid;

    void update();
};
