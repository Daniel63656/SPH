#include <cassert>
#include "simulation.h"
#include "datastructures/neighbourhood.h"

Simulation::Simulation(const Settings& settings, const KernelFunction* kernel) :
	m_kernel{ kernel },
	m_settings{ settings },
	m_grid{ settings }
{
	initializeParticles();
}

void Simulation::initializeParticles()
{
	//std::array<int, 2> nParticles = {};
	size_t nParticlesX = 0, nParticlesY = 0;

	double area = m_settings.physicalSize[0] * m_settings.physicalSize[1];
	nParticlesX = (int)std::lround(m_settings.physicalSize[0] * sqrt(m_settings.numberOfParticles / area));
	nParticlesY = m_settings.numberOfParticles / nParticlesX;

	std::array<double, 2> spacing{ m_settings.physicalSize[0] / nParticlesX, m_settings.physicalSize[1] / nParticlesY };

	for (int y = 0; y < nParticlesY; y++)
	{
		for (int x = 0; x < nParticlesX; x++)
		{
			std::cout << "pos " << x * spacing[0] + spacing[0]/2.0 << ", " << y * spacing[1] + spacing[1] / 2.0 << std::endl;
			m_particles.emplace_back(m_settings.mass, Vector<2>{x* spacing[0], y* spacing[1]}, Vector<2>{0, 0});
		}
	}

	refillGrid();

}

void Simulation::initializeBoundary()
{
    m_boundaryParticlesX = m_settings.boundaryDensity[0] * m_settings.physicalSize[0];
    m_boundaryParticlesY = m_settings.boundaryDensity[1] * m_settings.physicalSize[0];
    
    double pos = 0;
    for (int i = 0; i < m_boundaryParticlesX; i++){
        m_particles.emplace_back(0,Vec2{pos, 0}, Vec2{0,0}));
        pos += 1/m_settings.boundaryDensity[0];
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
	while (time < m_settings.endTime)
	{
		leap1();
		calculateDensityAndPressure();
		calculateForces();
		leap2();
		refillGrid();

		if (time >= next_write) {
			writer.write_vtp(m_particles);

			next_write += m_settings.vs_dt;
		}

		time += m_settings.dt;
		std::cout << "timestep t=" << time << "\n";
	}
}


Grid& Simulation::getGrid() {
	return m_grid;
}

void Simulation::calculateDensityAndPressure() {

	for (Particle& particle : m_particles)
	{
		double rho = 0;
		bool hasNeighbours = false;

		for (const auto& neighbour_particle : m_grid.neighbours(particle.position, m_kernel->effectiveRadius()))
		{
			hasNeighbours = true;
			rho += neighbour_particle.mass * m_kernel->W(particle.position - neighbour_particle.position);
		}

		assert(hasNeighbours);
		particle.rho = rho;
		particle.pressure = m_settings.kappa * (pow(rho / m_settings.rho_0, 7) - 1);
	}
}


void Simulation::calculateForces()
{
	for (auto& p_i : m_particles)
	{
		p_i.forces = p_i.rho * m_settings.g;
		for (const auto& p_j : m_grid.neighbours(p_i.position, m_kernel->effectiveRadius()))
		{
			double vol_i = p_i.mass / p_i.rho;
			double vol_j = p_j.mass / p_j.rho;

			p_i.forces += (p_i.pressure * vol_i + p_j.pressure * vol_j) / 2 * m_kernel->gradW(p_i.position - p_j.position)
				+ (p_i.velocity * vol_i - p_j.velocity * vol_j) / 2 * m_kernel->laplaceW(p_i.position - p_j.position);
		}
	}
}


void Simulation::leap1()
{
	const double half_dt = 0.5 * m_settings.dt;

	for (auto& p : m_particles)
	{
		p.position += p.velocity * half_dt;
	}
}

void Simulation::leap2()
{
	const double half_dt = 0.5 * m_settings.dt;
	for (auto& p : m_particles)
	{
		p.velocity += m_settings.dt / p.rho * p.forces;
		p.position += p.velocity * half_dt;
        std::cout << p.position << std::endl;
	}
}

void Simulation::refillGrid()
{
	//resort Particles into grid datastructures based on updated positions
	m_grid.clear();
	for (auto& p : m_particles)
	{
		m_grid.add(&p);
	}
}
