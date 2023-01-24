#pragma once

template<typename Vec>
struct Particle {
    double mass;
    double rho;
    double pressure;
    Vec position;
    Vec velocity;
};