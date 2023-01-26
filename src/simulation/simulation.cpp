#include "simulation.h"


template<unsigned int N>
Simulation<N>::Simulation(const Settings<N> &settings, const KernelFunction<N>* kernel) :
        m_kernel{kernel},
        m_settings{settings},
        time{settings.startTime}
{
    initializeParticles();
}


template<unsigned int N>
void Simulation<N>::run()
{
    while (time < m_settings.endTime) {
        calculateDensityAndPressure();
        calculateForces();
        updateParticles();

        time += m_settings.dt;
    }
}


template<unsigned int N>
void Simulation<N>::calculateDensityAndPressure() {

    for (Particle<N>& p_i : particles)
    {
        float rho = 0;
        for (Particle<N>& p_j : grid.neighbours(p_i, m_kernel->effectiveRadius()))
        {
            rho += p_j.mass * m_kernel->W(p_i.position - p_j.position);
        }

        p_i.rho = rho;
        p_i.pressure = m_settings.kappa*(pow(rho/m_settings.rho_0, 7) - 1);
    }
}


template<unsigned int N>
void Simulation<N>::calculateForces() {
    for (Particle<N>& p_i : particles)
    {
        p_i.forces = p_i.rho*m_settings.g;
        for (Particle<N>& p_j : grid.neighbours(p_i, m_kernel->effectiveRadius()))
        {
            double vol_i = p_i.mass/p_i.rho;
            double vol_j = p_j.mass/p_j.rho;

            p_i.forces += (p_i.pressure*vol_i + p_j.pressure*vol_j) / 2 * m_kernel->gradW(p_i.position - p_j.position)
                        + (p_i.velocity*vol_i - p_j.velocity*vol_j) / 2 * m_kernel->laplaceW(p_i.position - p_j.position);
        }
    }
}


template<unsigned int N>
void Simulation<N>::updateParticles() {
    for (Particle<N>& p_i : particles)
    {
        p_i.velocity += m_settings.dt/p_i.rho * p_i.forces;
        p_i.position += m_settings.dt*p_i.velocity;
    }

    //resort Particles into grid datastructure based on updated positions
    grid.clear();
    for (Particle<N> p : particles)
    {
        grid.add(p);
    }


    //calculate velocity field???
}


template class Simulation<2>;