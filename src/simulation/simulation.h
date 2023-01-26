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

    void initializeParticles();

    void run();


private:
    const Settings& m_settings;
    const KernelFunction* m_kernel;
    double time;

    Grid grid;
    std::vector<std::shared_ptr<Particle>> particles;

    void calculateDensityAndPressure();
    void calculateForces();
    void updateParticles();
};
