#pragma once

#include <iostream>
#include <vector>
#include "particle.h"

template <unsigned int N>
class Grid
{
public:
    Grid() {
        nCells = 10;
        for (int i = 0; i < N; i++)
        {
            meshWidth[i] = physicalSize[i] / size[i];
            nCells *= size[i];
        }

        grid.reserve(nCells);
        for (int i = 0; i < nCells; i++)
            grid.push_back(std::vector<Particle<N>>());
    }


    void clear();

    void add(Particle<N> p);

    std::vector<Particle<N>> neighbours(Particle<N> center, double radius);



private:
    int nCells;                             //number of total cells
    std::array<int, N> size;                //number of cells in all N spacial directions
    std::array<float, N> physicalSize;      //dimensions of the domain
    std::array<float, N> meshWidth;         //width of the cells in all N spacial directions

    std::vector<std::vector<Particle<N>>> grid;

    std::array<int, N> discretizedPosition(Vector<N> v);
    int pos2idx(std::array<int, N> pos);

    void searchDimension(int dim, std::array<int, N> start, std::array<int, N> end, int* idx,
                         Particle<N> center, double radius, std::vector<Particle<N>>& neighbours);
};
