#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "particle.h"
#include "../settings/settings.h"


class Grid
{
public:
    explicit Grid(const Settings& settings);

    void clear();
    void add(Particle* p);

    std::vector<Particle*> neighbours(std::shared_ptr<Particle> center, double radius);

private:
    const Settings& m_settings;

    int nTotal;                             //total number of cells
    std::array<double, 2> meshWidth{};      //width of the cells in all spacial directions
    std::vector<std::vector<Particle*>> grid;

    std::array<int, 2> discretizedPosition(Vector<2> v);
    int pos2idx(std::array<int, 2> pos);
};
