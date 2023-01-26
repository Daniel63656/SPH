#pragma once

#include <iostream>
#include <vector>
#include "particle.h"
#include "../settings/settings.h"


class Grid
{
public:
    Grid(const Settings& settings);
    void clear();
    void add(Particle p);
    std::vector<Particle> neighbours(Particle center, double radius);

private:
    const Settings& m_settings;             //access to simulation parameters
    int nCells;                             //number of total cells
    std::array<double, 2> meshWidth{};      //width of the cells in all N spacial directions

    std::vector<std::vector<Particle>> grid;

    std::array<int, 2> discretizedPosition(Vector<2> v);
    int pos2idx(std::array<int, 2> pos);
};
