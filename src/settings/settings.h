#pragma once

#include "../datastructures/vector.h"
#include "pch.h"

/*
 * All settings that parametrize a simulation run.
 */

struct Boundary
{
	Boundary(int nParticlesPerRow, double particleMass)
		: m_nParticlesPerRow(nParticlesPerRow), m_particleMass(particleMass)
	{
	}

	int m_nParticlesPerRow;
	int m_thickness = 1;
	double m_particleMass;
	Vec2d m_velocity = 0;
};
inline std::ostream& operator<<(std::ostream& os, const Boundary& boundary)
{
	return os << "{" << boundary.m_nParticlesPerRow << "x" << boundary.m_thickness
			  << " particles, particleMass=" << boundary.m_particleMass << ", velocity=" << boundary.m_velocity << "}";
}

enum KernelType
{
	GAUSSIAN,
	CUBIC_SPLINE
};
inline std::ostream& operator<<(std::ostream& os, const KernelType& kt)
{
	switch (kt)
	{
	case GAUSSIAN:
		return os << "Gaussian";
	case CUBIC_SPLINE:
		return os << "Cubic Spline";
	}
	return os;
}

enum Scenario
{
	LID_DRIVEN_CAVITY,
	KARMAN_VORTEX
};
inline std::ostream& operator<<(std::ostream& os, const Scenario& s)
{
	switch (s)
	{
	case LID_DRIVEN_CAVITY:
		return os << "Lid driven cavity";
	case KARMAN_VORTEX:
		return os << "Karman vortex";
	}
	return os;
}

struct Settings
{
	explicit Settings(const std::string& filename);
	Scenario scenario; //< scenario to simulate

	// constants
	double kappa;	//< gas constant
	double rho_0;	//< rest density at zero pressure
	double mass;	//< mass of an individual particle
	double mu;		//< kinematic viscosity of the fluid
	int nParticles; //< number of particles inside the domain

	// domain
	Vec2d physicalSize; //< dimensions of the domain
	Vec2i nCells{};		//< number of cells in all spacial directions
	Boundary bottom = Boundary(10, -1);
	Boundary top = Boundary(10, -1);
	Boundary left = Boundary(10, -1);
	Boundary right = Boundary(10, -1);

	// time
	double endTime = 10;
	double dt = 0.1;
	double vs_dt = 1;

	// miscellaneous
	Vec2d g; //< external forces
	KernelType kernelType;
	double smoothness;

	//! calculate parameters marked as auto and boundary particles
	void calculateParameters();

	//! output all settings to console
	void printSettings() const;

  private:
	static void line2nameAndValue(std::string line, std::string& paramName, std::string& paramValue);
};
