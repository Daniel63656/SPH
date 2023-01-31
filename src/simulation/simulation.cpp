#include <cassert>
#include "simulation.h"


Simulation::Simulation(const Settings &settings, const KernelFunction* kernel) :
        m_kernel{kernel},
        m_settings{settings},
        grid{settings}
{
    initializeParticles();
}

void Simulation::initializeParticles()
{
    //std::array<int, 2> nParticles = {};
    size_t domainSizeX = 0, domainSizeY = 0;

    double area = m_settings.physicalSize[0]*m_settings.physicalSize[1];
    domainSizeX = (int)std::lround(m_settings.physicalSize[0] * sqrt(m_settings.numberOfParticles/area));
    domainSizeY = m_settings.numberOfParticles/ domainSizeX;

    std::array<double, 2> spacing{m_settings.physicalSize[0]/ domainSizeX, m_settings.physicalSize[1]/ domainSizeY };

    for (int y = 0; y < domainSizeY; y++)
    {
        for (int x = 0; x < domainSizeX; x++)
        {
            particles.emplace_back(m_settings.mass, Vector<2>{x* spacing[0], y* spacing[1]}, Vector<2>{0, 0});
            grid.add(&particles.back());
        }
    }
}

void Simulation::run(VtkWriter *vtkWriter)
{
    double time = 0;
    while (time < m_settings.endTime) {
        calculateDensityAndPressure();
        calculateForces();
        updateParticles();

        time += m_settings.dt;
        std::cout << "timestep t=" << time << "\n";

        if (vtkWriter != nullptr) {
            vtkWriter->writeFile(time);
        }
    }
}


Grid& Simulation::getGrid() {
    return grid;
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

    //resort Particles into grid datastructures based on updated positions
    grid.clear();
    for (const std::shared_ptr<Particle>& p : particles)
    {
        grid.add(p);
    }
}
