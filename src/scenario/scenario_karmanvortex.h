#pragma once

#include "simulation/simulation.h"

class KarmanVortex : public Simulation
{
public:
    KarmanVortex(const Settings &settings, const std::shared_ptr<KernelFunction> &kernel, MPI_Vars &mpiInfo);

private:
    Vec2i nParticles;
    Vec2d spacing;

    bool insideObstacle(Vec2d pos);
    const Vec2d obstaclePos = Vec2d({2, 1});
    const double obstacleRadius = 0.4;

    void initialize() override;
    void calculateForces() override;

    void initializeParticles();
    void initializeBoundaries();
};