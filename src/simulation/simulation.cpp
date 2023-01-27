#include <cassert>
#include "simulation.h"


Simulation::Simulation(const Settings &settings, const KernelFunction* kernel) :
        m_kernel{kernel},
        m_settings{settings},
        time{settings.startTime},
        grid{settings}
{
    initializeParticles();
}

void Simulation::initializeParticles()
{
    std::array<int, 2> nParticles = {};
    double area = m_settings.physicalSize[0]*m_settings.physicalSize[1];
    nParticles[0] = (int)std::lround(m_settings.physicalSize[0] * sqrt(m_settings.numberOfParticles/area));
    nParticles[1] = m_settings.numberOfParticles/nParticles[0];

    std::array<double, 2> spacing{m_settings.physicalSize[0]/nParticles[0], m_settings.physicalSize[1]/nParticles[1]};
    for (int y = 0; y < nParticles[1]; y++)
    {
        for (int x = 0; x < nParticles[0]; x++)
        {
            std::shared_ptr<Particle> p = std::make_shared<Particle>(m_settings.mass, Vector<2>{x*spacing[0], y*spacing[1]}, Vector<2>{0, 0});
            particles.push_back(p);
            grid.add(p);
        }
    }
}

void Simulation::run()
{
    while (time < m_settings.endTime) {

        if (time > 3.2)
            std::cout << "*********************\n";

        calculateDensityAndPressure();
        calculateForces();
        updateParticles();

        time += m_settings.dt;
        std::cout << "timestep t=" << time << "\n";
    }
}


void Simulation::calculateDensityAndPressure() {

    for (const std::shared_ptr<Particle>& p_i : particles)
    {
        double rho = 0;
        bool hasNeighbours = false;
        for (const std::shared_ptr<Particle>& p_j : grid.neighbours(p_i, m_kernel->effectiveRadius()))
        {
            hasNeighbours = true;
            rho += p_j->mass * m_kernel->W(p_i->position - p_j->position);
        }

        assert(hasNeighbours);
        p_i->rho = rho;
        p_i->pressure = m_settings.kappa*(pow(rho/m_settings.rho_0, 7) - 1);
    }
}


void Simulation::calculateForces() {
    for (const std::shared_ptr<Particle>& p_i : particles)
    {
        p_i->forces = p_i->rho*m_settings.g;
        for (const std::shared_ptr<Particle>& p_j : grid.neighbours(p_i, m_kernel->effectiveRadius()))
        {
            double vol_i = p_i->mass/p_i->rho;
            double vol_j = p_j->mass/p_j->rho;

            p_i->forces += (p_i->pressure*vol_i + p_j->pressure*vol_j) / 2 * m_kernel->gradW(p_i->position - p_j->position)
                         + (p_i->velocity*vol_i - p_j->velocity*vol_j) / 2 * m_kernel->laplaceW(p_i->position - p_j->position);
        }
    }
}


void Simulation::updateParticles() {
    for (const std::shared_ptr<Particle>& p_i : particles)
    {
        p_i->velocity += m_settings.dt/p_i->rho * p_i->forces;
        p_i->position += m_settings.dt*p_i->velocity;
    }

    //resort Particles into grid datastructure based on updated positions
    grid.clear();
    for (const std::shared_ptr<Particle>& p : particles)
    {
        grid.add(p);
    }


    //calculate velocity field???
}