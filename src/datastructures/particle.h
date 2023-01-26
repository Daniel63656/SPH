#pragma once

#include "vector.h"

struct Particle
{
    Particle(double mass, const Vector<2> &position, const Vector<2> &velocity) :
             mass(mass),
             position(position),
             velocity(velocity)
             {}

    double mass;
    double rho{};
    double pressure{};
    Vector<2> position;
    Vector<2> velocity;

    Vector<2> forces;       //vector to store calculated forces in each time run
};