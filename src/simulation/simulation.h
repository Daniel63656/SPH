#pragma once

#include <vector>
#include <memory>
#include "../datastructures/grid.h"
#include "../kernelFunctions/kernelFunction.h"
#include "../settings/settings.h"
#include "../kernelFunctions/gaussian.h"
#include "../output_writer/vtk_writer.h"


class Simulation
{
public:
    Simulation(const Settings& settings, const KernelFunction* kernel);

    void initializeParticles();


    //! run the simulation
    //! @param vtkWriter optional: provide a VtkWriter so a paraview output gets created
    void run(VtkWriter* vtkWriter = nullptr);

    Grid& getGrid();


private:
    const Settings& m_settings;
    const KernelFunction* m_kernel;

    Grid grid;
    std::vector<std::shared_ptr<Particle>> particles;

    void calculateDensityAndPressure();
    void calculateForces();
    void updateParticles();
};
