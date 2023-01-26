#pragma once

#include <vector>
#include <memory>
#include "../datastructures/grid.h"
#include "../kernelFunctions/kernelFunction.h"
#include "../settings/settings.h"
#include "../kernelFunctions/gaussian.h"


class Simulation
{
public:
    Simulation(const Settings& settings, const KernelFunction* kernel);

    void run();


private:
    const Settings& m_settings;
    const KernelFunction* m_kernel;
    double time;

    std::vector<Particle> particles;
    Grid grid;

    void initializeParticles();

    void calculateDensityAndPressure();
    void calculateForces();
    void updateParticles();
};
