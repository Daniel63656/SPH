#pragma once

#include "scenario.h"
#include "simulation/simulation.h"

class LidDrivenCavity : public Scenario
{
private:
	void initializeParticles(Simulation* sim);
	void initializeBoundaries(Simulation* sim);
public:
	// Inherited via Scenario
	void init(Simulation* sim) override;
	void update(Simulation* sim) override;
};