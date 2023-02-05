#pragma once
#include "pch.h"

class Simulation;

class Scenario
{
public:
	virtual void init(Simulation* sim) = 0;
	virtual void update(Simulation* sim) = 0;
};