#pragma once

#include <vector>
#include <memory>
#include "../datastructures/grid.h"
#include "../kernelFunctions/kernelFunction.h"
#include "../settings/settings.h"
#include "../kernelFunctions/gaussian.h"

template<unsigned int N>
class Simulation
{
public:
    Simulation(const Settings<N>& settings, const KernelFunction<N>* kernel);

    void run();


private:
    const Settings<N>& m_settings;
    const KernelFunction<N>* m_kernel;
    double time;

    std::vector<Particle<N>> particles;
    Grid<N> grid;

    void initializeParticles() {
        //TODO initialize specified number of particles uniformly across domain
    }

    void calculateDensityAndPressure();
    void calculateForces();
    void updateParticles();
};
