#include <cassert>
#include "simulation.h"
#include "datastructures/neighbourhood.h"

Simulation::Simulation(const Settings& settings, const KernelFunction* kernel, MPI_Vars& mpi_info) :
	m_kernel{ kernel },
	m_settings{ settings },
	m_grid{ settings },
	m_mpi_info(mpi_info)
{
	initializeParticles();
	//initializeBoundary();

	m_mpi_info.arrayend = m_particles.size() + m_boundaryparticles.size();


}

void Simulation::initializeParticles()
{
	//std::array<int, 2> nParticles = {};
	size_t nParticlesX = 0, nParticlesY = 0;

	double b_thickness = m_settings.boundaryThickness;

	double area = m_settings.physicalSize.x * m_settings.physicalSize.y;
	double boundary_area = (m_settings.physicalSize.x + 2 * b_thickness) * (m_settings.physicalSize.y + 2 * b_thickness);

	int numberOfParticles = (int)(m_settings.particleDensity * boundary_area);
	std::cout << "# of particles: " << numberOfParticles << std::endl;
	nParticlesX = (int)std::lround(m_settings.physicalSize.x * sqrt(numberOfParticles / area));
	nParticlesY = numberOfParticles / nParticlesX;

	Vec2d spacing{ (m_settings.physicalSize.x + 2 * b_thickness) / nParticlesX, (m_settings.physicalSize.y + 2 * b_thickness) / nParticlesY };
    bool flag = true;

	for (int y = 0; y < nParticlesY; y++)
	{
		for (int x = 0; x < nParticlesX; x++)
		{
			double xPos = x * spacing.x + spacing.x / 2.0;
			double yPos = y * spacing.y + spacing.y / 2.0;

			if (xPos > b_thickness && xPos < m_settings.physicalSize.x + b_thickness && yPos > b_thickness && yPos < m_settings.physicalSize.y + b_thickness)
				m_particles.emplace_back(m_settings.mass, Vec2d(xPos, yPos), Vec2d(0, 0));
			else
				m_boundaryparticles.emplace_back(m_settings.mass, Vec2d(xPos, yPos), Vec2d(0, 0));
            if (yPos >= m_settings.physicalSize.y+b_thickness && flag){
                m_upperParticle = m_boundaryparticles.size()-1;
                flag = false;
            }
		}
	}
    std::cout << m_upperParticle << "," << m_boundaryparticles.size() << std::endl;


}


void Simulation::updateBoundary()
{
	const double half_dt = 0.5 * m_settings.dt;

	for (int i = m_upperParticle; i < m_boundaryparticles.size(); i++) {
		Vec2d& pos = m_boundaryparticles[i].position;
		pos.x = pos.x + half_dt;
		if (pos.x > m_settings.physicalSize.x + m_settings.boundaryThickness*2)
			pos.x = -(pos.x - (m_settings.physicalSize.x + m_settings.boundaryThickness*2));

        m_boundaryparticles[i].velocity.x = 1;
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
	//
	writer.build_tree();
	auto out = m_particles;
	out.insert(out.end(), m_boundaryparticles.begin(), m_boundaryparticles.end());
	writer.write_vtp(out);

	double time = 0;
	double next_write = m_settings.vs_dt;
	refillGrid();
	while (time < m_settings.endTime)
	{
		leap1();
        updateBoundary();
		calculateDensityAndPressure();
		calculateForces();
		leap2();
        updateBoundary();
		refillGrid();

		if (time >= next_write) {
			auto out = m_particles;
			out.insert(out.end(), m_boundaryparticles.begin(), m_boundaryparticles.end());
			writer.write_vtp(out);

			next_write += m_settings.vs_dt;
		}

		time += m_settings.dt;
		std::cout << "timestep t=" << time << "\n";

		//if (time > 0.02)
		//    break;
	}
}


Grid& Simulation::getGrid() {
	return m_grid;
}

void Simulation::calcDensityPresure(Particle& particle)
{
	double rho = 0;
	bool hasNeighbours = false;

	for (const auto neighbour_particle : m_grid.neighbours(particle.position, m_kernel->effectiveRadius()))
	{
		hasNeighbours = true;
		rho += neighbour_particle->mass * m_kernel->W(particle.position - neighbour_particle->position);
	}

	assert(hasNeighbours);
	particle.density = rho;
	particle.pressure = m_settings.kappa * (pow(rho / m_settings.rho_0, 7) - 1);
}

void Simulation::calculateDensityAndPressure() {

	for (Particle& particle : m_particles)
	{
		calcDensityPresure(particle);
	}

	for (Particle& particle : m_boundaryparticles)
	{
		calcDensityPresure(particle);
	}
}


void Simulation::calculateForces()
{
	for (auto& p_i : m_particles)
	{
		p_i.forces = p_i.density * m_settings.g;
		for (const auto p_j : m_grid.neighbours(p_i.position, m_kernel->effectiveRadius()))
		{
			double vol_i = p_i.mass / p_i.density;
			double vol_j = p_j->mass / p_j->density;

			/*if (p_j->position.y == 0 || p_j->position.x == 0){
				std::cout << p_j->position.serialize(2) << ". " << (p_i.pressure * vol_i + p_j->pressure * vol_j) / 2 * m_kernel->gradW(p_i.position - p_j->position)
				+ (p_i.velocity * vol_i - p_j->velocity * vol_j) / 2 * m_kernel->laplaceW(p_i.position - p_j->position) << std::endl;
			}*/

			//p_i.forces += (p_i.pressure * vol_i + p_j->pressure * vol_j) / 2 * m_kernel->gradW(p_i.position - p_j->position)
			//	- (p_i.velocity * vol_i - p_j->velocity * vol_j) / 2 * m_kernel->laplaceW(p_i.position - p_j->position);


			Vec2d pressure = -(1.0 / 2.0) * (p_i.pressure * (p_i.mass / p_i.density) + p_j->pressure * (p_j->mass / p_j->density)) * m_kernel->gradW(p_i.position - p_j->position);
			Vec2d viscosity = (m_settings.mu / 2.0) * (p_j->velocity * (p_j->mass / p_j->density) - p_i.velocity* (p_i.mass / p_i.density)) * m_kernel->laplaceW(p_i.position - p_j->position);

			p_i.forces += pressure + viscosity;

		}
	}
}


void Simulation::leap1()
{
	const double half_dt = 0.5 * m_settings.dt;
	std::cout << std::endl;
	for (auto& p : m_particles)
	{
		p.position += p.velocity * half_dt;

		//std::cout << p.position << std::endl;
	}
}

void Simulation::leap2()
{
	const double half_dt = 0.5 * m_settings.dt;
	for (auto& p : m_particles)
	{

		//std::cout << "\n" << p.position << std::endl;
		p.velocity += (p.forces / p.density) * m_settings.dt;
		p.position += p.velocity * half_dt;
		//std::cout << p.velocity << std::endl;
		//std::cout << p.position << std::endl;
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
	for (auto& p : m_boundaryparticles)
	{
		m_grid.add(&p);
	}
}
