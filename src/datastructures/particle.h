#pragma once

#include "vector.h"

template <unsigned int N>
struct Particle {
    double mass;
    double rho;
    double pressure;
    Vector<N> position;
    Vector<N> velocity;

    Vector<N> forces;       //vector to store calculated forces in each time step
};