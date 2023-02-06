#pragma once

#include "scenario.h"
#include "simulation/simulation.h"

class KarmanVortex : public Scenario
{
private:
	void initializeParticles(Simulation* sim);
	void initializeBoundaries(Simulation* sim);

    bool insideObstacle(Vec2d pos);

    Vec2i nParticles;
    Vec2d spacing;

    const Vec2d obstaclePos = Vec2d({2, 1});
    const double obstacleRadius = 0.4;
public:
	// Inherited via Scenario
	void init(Simulation* sim) override;
	void update(Simulation* sim) override;
};