#pragma once

#include "simulation/simulation.h"

class KarmanVortex : public Simulation
{
public:
    KarmanVortex(const Settings &settings, const std::shared_ptr<KernelFunction> &kernel);

private:
    Vec2i nParticles;
    Vec2d spacing;

    double velocity = 0.45;    //velocity in x direction of stream
    double respawnTime;
    int timeIt = 1;

    bool insideObstacle(Vec2d pos);
    const Vec2d obstaclePos = Vec2d({3, 1});
    const double obstacleRadius = 0.3;

    void initialize() override;
    void calculateForces() override;
    void update() override;

    void initializeParticles();
    void initializeBoundaries();
};
