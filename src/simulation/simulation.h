#pragma once

#include "../pch.h"
#include "../datastructures/grid.h"
#include "../kernelFunctions/kernelFunction.h"
#include "../settings/settings.h"
#include "../kernelFunctions/gaussian.h"
#include "../output_writer/outputwriter.h"
#include "scenario/scenario.h"
#include "scenario/scenario_fillingbucket.h"
class Simulation
{
public:
    friend class LidDrivenCavity;
    friend class KarmanVortex;
    friend class FillingBucket;

    Simulation(const Settings& settings, std::shared_ptr<KernelFunction> kernel, std::shared_ptr<Scenario> scenario, MPI_Vars& mpi_info);

    //! run the simulation
    //! @param vtkWriter optional: provide a VtkWriter so a paraview output gets created
    void run(OutputWriter& writer);

    Grid& getGrid();

private:
    const Settings& m_settings;
    std::shared_ptr<KernelFunction> m_kernel;

    Grid m_grid;
    std::vector<Particle> m_particles;
    std::vector<Particle> m_boundaryParticles;
    MPI_Vars& m_mpi_info;

    std::shared_ptr<Scenario> m_scenario;

    void calcDensityPresure(Particle& particle);
    void calculateDensityAndPressure();
    void calculateForces();
    void leapfrog(bool firstIteration);
    void refillGrid();
};
