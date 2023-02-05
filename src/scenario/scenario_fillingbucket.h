#pragma once

#include "scenario.h"
#include "simulation/simulation.h"

class FillingBucket : public Scenario
{
private:
	void initializeParticles(Simulation* sim);
	void initializeBoundaries(Simulation* sim);
	const double m_fillrate = 0.5;
public:
	// Inherited via Scenario
	virtual void init(Simulation* sim) override;
	virtual void update(Simulation* sim) override;
};