#pragma once

#include <array>
#include <string>
#include "../datastructures/vector.h"

/*
 * All settings that parametrize a simulation run.
 */

enum Kernel {
    CUBIC_SPLINE, GAUSSIAN
};

template<unsigned int N>
struct Settings
{
    // constants
    double kappa = 4;                           //< gas constant
    double rho_0 = 3;                           //< rest density at zero pressure
    double m = 10.0;                            //< mass of an individual particle
    double mu = 4;                              //< kinematic viscosity of the fluid
    int numberOfParticles = 100;

    // domain
    std::array<double, 2> physicalSize{};         //< physical size of the domain
    //TODO boundary consditions

    // time
    double startTime = 0;
    double endTime = 10;
    double dt = 0.1;

    //miscellaneous
    Vector<N> g;                          //< external forces
    Kernel kernelFunction = GAUSSIAN;


    //! parse a text file with settings, each line contains "<parameterName> = <value>"
    void loadFromFile(std::string filename);

    //! output all settings to console
    void printSettings();
};
