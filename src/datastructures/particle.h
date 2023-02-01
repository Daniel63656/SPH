#pragma once

#include "vector.h"

struct Particle
{
    Particle(double mass, const Vec2 &position, const Vec2 &velocity) :
             mass(mass),
             position(position),
             velocity(velocity)
             {}

    double mass;
    double rho{};
    double pressure{};
    Vec2 position;
    Vec2 velocity;

    Vec2 forces;       //vector to store calculated forces in each time run
};