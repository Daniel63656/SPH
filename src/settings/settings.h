#pragma once

#include <array>
#include <string>

/*
 * All settings that parametrize a simulation run.
 */
enum Kernel
{
    GAUSSIAN
};


struct Settings
{
    std::array<double, 2> physicalSize;         //< physical size of the domain
    double kappa = 4;                           //< gas constant
    double m = 10.0;                            //< mass of an individual particle
    double mu = 4;                              //< kinematic viscosity of the fluid
    double dt = 0.1;                            //<

    std::array<double, 2> g{0., 0.};            //< external forces
    Kernel kernelFunction = Kernel::GAUSSIAN;   //< specifies which kernel function to use //TODO make class


    std::array<double, 2> dirichletBcBottom; //< prescribed values of u,v at bottom of domain
    std::array<double, 2> dirichletBcTop;    //< prescribed values of u,v at top of domain
    std::array<double, 2> dirichletBcLeft;   //< prescribed values of u,v at left of domain
    std::array<double, 2> dirichletBcRight;  //< prescribed values of u,v at right of domain

    //! parse a text file with settings, each line contains "<parameterName> = <value>"
    void loadFromFile(std::string filename);

    //! output all settings to console
    void printSettings();
};
