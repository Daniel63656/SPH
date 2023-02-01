#pragma once

#include "../pch.h"
#include "../datastructures/grid.h"
#include "../kernelFunctions/kernelFunction.h"
#include "../settings/settings.h"
#include "../kernelFunctions/gaussian.h"
#include "../output_writer/outputwriter.h"
//#include "../output_writer/vtk_writer.h"


class Simulation
{
public:
    Simulation(const Settings& settings, const KernelFunction* kernel);

    void initializeParticles();


    //! run the simulation
    //! @param vtkWriter optional: provide a VtkWriter so a paraview output gets created
    void run(OutputWriter& writer);

    Grid& getGrid();


private:
    const Settings& m_settings;
    const KernelFunction* m_kernel;

    Grid grid;
    std::vector<Particle> particles;

    void calculateDensityAndPressure();
    void calculateForces();
    //void updateParticles();

    void leap1();
    void leap2();

    void regenerateGrid();

};
