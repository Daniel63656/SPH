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
    Simulation(const Settings& settings, const KernelFunction* kernel, MPI_Vars& mpi_info);

    void initializeParticles();

    void initializeBoundary();

    void updateBoundary();

    //! run the simulation
    //! @param vtkWriter optional: provide a VtkWriter so a paraview output gets created
    void run(OutputWriter& writer);

    Grid& getGrid();

private:
    const Settings& m_settings;
    const KernelFunction* m_kernel;

    Grid m_grid;
    std::vector<Particle> m_particles;
    std::vector<Particle> m_boundaryparticles;
    int m_boundaryParticlesX;
    int m_boundaryParticlesY;
    void calcDensityPresure(Particle& particle);
    void calculateDensityAndPressure();
    void calculateForces();
    //void updateParticles();
    MPI_Vars& m_mpi_info;

    void leap1();
    void leap2();

    void refillGrid();
};
