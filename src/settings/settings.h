#pragma once

#include <array>
#include <string>
#include "../datastructures/vector.h"

/*
 * All settings that parametrize a simulation run.
 */


struct Settings
{
    // constants
    double kappa = 4;                           //< gas constant
    double rho_0 = 3;                           //< rest density at zero pressure
    double mass = 10.0;                         //< mass of an individual particle
    double mu = 4;                              //< kinematic viscosity of the fluid
    int numberOfParticles = 100;

    // domain
    Vec2d physicalSize;         //< dimensions of the domain
    Vec2i nCells{};                //< number of cells in all spacial directions
    Vec2d boundaryDensity;
    //TODO boundary conditions

    // time
    double endTime = 10;
    double dt = 0.1;
    double vs_dt = 1;

    //miscellaneous
    Vec2d g;                          //< external forces
    std::string kernelFunction = "GAUSSIAN";
    double smoothness;


    //! parse a text file with settings, each line contains "<parameterName> = <value>"
    void loadFromFile(const std::string& filename);

    //! output all settings to console
    void printSettings();
};
