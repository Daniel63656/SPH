#pragma once

#include <vector>
#include "../datastructures/grid.h"
#include "../kernelFunctions/kernelFunction.h"
#include "../settings/settings.h"

template<class K, unsigned int N>
class Simulation
{
public:
    Simulation(Settings<K, N>& settings);
    void step(double dt);

private:
    const Settings<K, N> &m_settings;
    double time;

    std::vector<Particle<N>> particles;
    Grid<N> grid;
    K kernel;

    void calculateDensityAndPressure();
    void calculateForces();
    void updateParticles();
};
