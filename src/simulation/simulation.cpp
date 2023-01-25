#include "simulation.h"


template<class K, unsigned int N>
Simulation<K, N>::Simulation(Settings<K, N> &settings) :
    m_settings{settings},
    time{settings.startTime}
    {}

template<class K, unsigned int N>
void Simulation<K, N>::step(double dt) {
    while (time < m_settings.endTime) {
        calculateDensityAndPressure();
        calculateForces();
        updateParticles();

        time += dt;
    }
}

template<class K, unsigned int N>
void Simulation<K, N>::calculateDensityAndPressure() {

    for (Particle<N> p_i : particles)
    {
        float rho = 0;
        for (Particle<N> p_j : grid.neighbours(p_i, kernel.effectiveRadius()))
        {
            rho += p_j.mass * kernel.W(euclideanDistance(p_i.position, p_j.position));
        }

        p_i.density = rho;
        p_i.pressure = m_settings.kappa*(pow(rho/m_settings.rho_0, 7) - 1);
    }
}

template<class K, unsigned int N>
void Simulation<K, N>::calculateForces() {
    for (Particle<N> p_i : particles)
    {
        p_i.forces = p_i.rho*m_settings.g;
        for (Particle<N> p_j : grid.neighbours(p_i, kernel.effectiveRadius()))
        {
            double vol_i = p_i.mass/p_i.rho;
            double vol_j = p_j.mass/p_j.rho;

            p_i.forces += (p_i.pressure*vol_i + p_j.pressure*vol_j)/2 *kernel.dW()
                        + (p_i.velocity*vol_i - p_j.velocity*vol_j)/2 *kernel.d2W();
        }
    }
}

template<class K, unsigned int N>
void Simulation<K, N>::updateParticles() {
    for (Particle<N> p_i : particles)
    {
        p_i.velocity += m_settings.dt/p_i.rho * p_i.forces;
        p_i.position += m_settings.dt*p_i.velocity;
    }

    //create a new Grid based on updated particle positions

    //calculate velocity field???
}