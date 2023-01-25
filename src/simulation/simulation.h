#pragma once

#include <vector>
#include <memory>
#include "../datastructures/grid.h"
#include "../kernelFunctions/kernelFunction.h"
#include "../settings/settings.h"
#include "../kernelFunctions/gaussian.h"

template<unsigned int N, typename K>
class Simulation
{
public:
    Simulation(Settings<N>& settings, const std::shared_ptr<KernelFunction<N>>& kernel) :
            m_kernel{kernel},
            m_settings{settings},
            time{settings.startTime}
    {
        initializeParticles();
    }

    void run();


private:
    const Settings<N>& m_settings;
    const std::shared_ptr<KernelFunction<N>>& m_kernel;
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
