#pragma once

#include <array>
#include <string>
#include "../datastructures/vector.h"

/*
 * All settings that parametrize a simulation run.
 */

struct Boundary
{
    Boundary(int nParticlesPerRow, double particleMass) : m_nParticlesPerRow(nParticlesPerRow), m_particleMass(particleMass) {}

    int m_nParticlesPerRow;
    int m_thickness = 1;
    double m_particleMass;
    Vec2d m_velocity = 0;
};
inline std::ostream& operator<< (std::ostream& os, const Boundary& boundary) {
    return os << "{" << boundary.m_nParticlesPerRow << "x" << boundary.m_thickness << " particles, particleMass="
              << boundary.m_particleMass << ", velocity=" << boundary.m_velocity << "}";
}


struct Settings
{
    explicit Settings(const std::string& filename);
    // constants
    double kappa = 4;                           //< gas constant
    double rho_0 = 3;                           //< rest density at zero pressure
    double mass = 10.0;                         //< mass of an individual particle
    double mu = 4;                              //< kinematic viscosity of the fluid
    int nParticles = 100;

    // domain
    Vec2d physicalSize;                         //< dimensions of the domain
    Vec2i nCells{};                             //< number of cells in all spacial directions
    double boundaryThickness;
    Boundary bottom = Boundary(10, mass);
    Boundary    top = Boundary(10, mass);
    Boundary   left = Boundary(10, mass);
    Boundary  right = Boundary(10, mass);
    
    // time
    double endTime = 10;
    double dt = 0.1;
    double vs_dt = 1;

    //miscellaneous
    Vec2d g;                          //< external forces
    std::string kernelFunction = "GAUSSIAN";
    double smoothness;


    void calculateSettings();

    //! output all settings to console
    void printSettings() const;

private:
    static void line2nameAndValue(std::string line, std::string& paramName, std::string& paramValue);
};
