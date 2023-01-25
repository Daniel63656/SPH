#pragma once

#include <iostream>
#include <vector>
#include "particle.h"

template <unsigned int N>
class Grid
{
public:
    Grid();
    void clear();

    void add(Particle<N> p);
    void remove(Particle<N> p);

    std::vector<Particle<N>> neighbours(Particle<N> center, float radius);



private:
    int nCells;                             //number of total cells
    std::array<int, N> size;                //number of cells in all N spacial directions
    std::array<float, N> physicalSize;      //dimensions of the domain
    std::array<float, N> meshWidth;         //width of the cells in all N spacial directions

    std::vector<std::vector<Particle<N>>> grid;

    int calcIndex(Vector<N> v);
    int pos2idx(Vector<N> pos);
};
