#pragma once

#include "../pch.h"
#include "../datastructures/grid.h"
#include "../kernelFunctions/kernelFunction.h"
#include "../settings/settings.h"
#include "../kernelFunctions/gaussian.h"
#include "../output_writer/outputwriter.h"
class Simulation
{
public:
    friend class LidDrivenCavity;
    friend class KarmanVortex;
    friend class FillingBucket;

    Simulation(const Settings& settings, std::shared_ptr<KernelFunction> kernel, MPI_Vars& mpi_info);

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

    virtual void initialize();
    virtual void calculateDensityAndPressure();
    virtual void calculateForces();

    void calculateDensityAndPressure(Particle& particle);
    void leapfrog(bool firstIteration);
    void refillGrid();

    void initializeParticles();
    void initializeBoundaries();
};
