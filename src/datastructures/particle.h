#pragma once

#include "vector.h"

struct Particle
{
    Particle(double mass, const Vec2d&position, const Vec2d&velocity) :
             mass(mass),
             position(position),
             velocity(velocity)
             {}

    double mass;
    double rho = 0;
    double pressure = 0;
    Vec2d position;
    Vec2d velocity;

    Vec2d forces;       //vector to store calculated forces in each time run
};