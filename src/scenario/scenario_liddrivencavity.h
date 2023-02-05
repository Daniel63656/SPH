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
	virtual void init(Simulation* sim) override;
	virtual void update(Simulation* sim) override;
};