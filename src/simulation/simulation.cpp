#include <cassert>
#include "simulation.h"


Simulation::Simulation(const Settings& settings, const KernelFunction* kernel) :
	m_kernel{ kernel },
	m_settings{ settings },
	grid{ settings }
{
	initializeParticles();
}

void Simulation::initializeParticles()
{
	//std::array<int, 2> nParticles = {};
	size_t domainSizeX = 0, domainSizeY = 0;

	double area = m_settings.physicalSize[0] * m_settings.physicalSize[1];
	domainSizeX = (int)std::lround(m_settings.physicalSize[0] * sqrt(m_settings.numberOfParticles / area));
	domainSizeY = m_settings.numberOfParticles / domainSizeX;

	std::array<double, 2> spacing{ m_settings.physicalSize[0] / domainSizeX, m_settings.physicalSize[1] / domainSizeY };

    for (int y = 0; y < domainSizeY; y++)
    {
        for (int x = 0; x < domainSizeX; x++)
        {
            particles.emplace_back(m_settings.mass, Vector<2>{x* spacing[0], y* spacing[1]}, Vector<2>{0, 0});
            grid.add(&particles.back());
        }
    }
}

void Simulation::run(OutputWriter& writer)
{
	// testing neighbourhood search / iterator
	
	//particles.emplace_back(m_settings.mass, Vec2{ 1.0, 0.0 }, Vec2{ 0, 0 });
	//particles.emplace_back(m_settings.mass, Vec2{ 2.0, 0.0 }, Vec2{ 0, 0 });
	//particles.emplace_back(m_settings.mass, Vec2{ 3.0, 0.0 }, Vec2{ 0, 0 });
	//particles.emplace_back(m_settings.mass, Vec2{ 0.5, 0.0 }, Vec2{ 0, 0 });
	//particles.emplace_back(m_settings.mass, Vec2{ 0.1, 0.7 }, Vec2{ 0, 0 });

	////find these
	//particles.emplace_back(m_settings.mass, Vec2{ 3.0, 2.0 }, Vec2{ 0, 0 });
	//particles.emplace_back(m_settings.mass, Vec2{ 2.0, 2.5 }, Vec2{ 0, 0 });

	//for (auto& a : particles)
	//{
	//	grid.add(&a);
	//}

	//for (auto& p : grid.neighbours(Vec2{ 3.0,2.5 }, 1.0))
	//{
	//	std::cout << "pos " << p.position[0] << ", " << p.position[1] << std::endl;
	//}
	

    double time = 0;
    double next_write = m_settings.vs_dt;
    while (time < m_settings.endTime) {
        calculateDensityAndPressure();
        calculateForces();
        updateParticles();

        if (time >= next_write){
            writer.write_vtp(particles);

            next_write += m_settings.vs_dt;
        }

        time += m_settings.dt;
        std::cout << "timestep t=" << time << "\n";
    }
}


Grid& Simulation::getGrid() {
	return grid;
}

void Simulation::calculateDensityAndPressure() {

	for (Particle& p_i : particles)
	{
		double rho = 0;
		bool hasNeighbours = false;
		//for (const Particle* p_j : grid.neighbours(p_i, m_kernel->effectiveRadius()))
		//{
		//	hasNeighbours = true;
		//	rho += p_j->mass * m_kernel->W(p_i.position - p_j->position);
		//}

		for (auto particle : grid.neighbours(p_i.position, m_kernel->effectiveRadius()))
		{
			hasNeighbours = true;
			rho += particle.mass * m_kernel->W(p_i.position - particle.position);
		}

		assert(hasNeighbours);
		p_i.rho = rho;
		p_i.pressure = m_settings.kappa * (pow(rho / m_settings.rho_0, 7) - 1);
	}
}


void Simulation::calculateForces() {
	for (Particle& p_i : particles)
	{
		p_i.forces = p_i.rho * m_settings.g;
		for (auto p_j : grid.neighbours(p_i.position, m_kernel->effectiveRadius()))
		{
			double vol_i = p_i.mass / p_i.rho;
			double vol_j = p_j.mass / p_j.rho;

			p_i.forces += (p_i.pressure * vol_i + p_j.pressure * vol_j) / 2 * m_kernel->gradW(p_i.position - p_j.position)
				+ (p_i.velocity * vol_i - p_j.velocity * vol_j) / 2 * m_kernel->laplaceW(p_i.position - p_j.position);
		}
	}
}


void Simulation::updateParticles() {
	for (Particle& p_i : particles)
	{
		p_i.velocity += m_settings.dt / p_i.rho * p_i.forces;
		p_i.position += m_settings.dt * p_i.velocity;
	}

	//resort Particles into grid datastructures based on updated positions
	grid.clear();
	for (Particle& p : particles)
	{
		grid.add(&p);
	}
}
