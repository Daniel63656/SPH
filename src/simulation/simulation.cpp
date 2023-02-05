#include <cassert>
#include "simulation.h"
#include "datastructures/neighbourhood.h"

Simulation::Simulation(const Settings& settings, const KernelFunction* kernel, MPI_Vars& mpi_info) :
	m_kernel(kernel),
	m_settings(settings),
	m_grid(settings),
	m_mpi_info(mpi_info)
{
	initializeParticles();
    initializeBoundaries();

	m_mpi_info.arrayend = m_particles.size() + m_boundaryparticles.size();


}

void Simulation::initializeParticles()
{
    Vec2i nParticles;
    double area = m_settings.physicalSize.x*m_settings.physicalSize.y;
    nParticles.x = (int)std::lround(m_settings.physicalSize.x * sqrt(m_settings.nParticles / area));
    nParticles.y = m_settings.nParticles / nParticles.x;

    std::array<double, 2> spacing{m_settings.physicalSize.x/(nParticles.x+1), m_settings.physicalSize.y/(nParticles.y+1)};
    for (int y = 0; y < nParticles.y; y++)
    {
        for (int x = 0; x < nParticles.x; x++)
        {
            m_particles.emplace_back(m_settings.mass, Vec2d((x+1)*spacing[0], (y+1)*spacing[1]), Vec2d(0, 0));
        }
    }
}

void Simulation::initializeBoundaries()
{
    //bottom
    double particleDensity = m_settings.physicalSize.x/(m_settings.bottom.m_nParticlesPerRow - 1);
    for (int t = 0; t < m_settings.bottom.m_thickness; t++) {
        for (int i = 0; i < m_settings.bottom.m_nParticlesPerRow; i++) {
            m_boundaryparticles.emplace_back(m_settings.bottom.m_particleMass, Vec2d(particleDensity * i, -particleDensity*t),
                                             Vec2d(m_settings.bottom.m_velocity.x, m_settings.bottom.m_velocity.y));
        }
    }
    //top
    particleDensity = m_settings.physicalSize.x/(m_settings.top.m_nParticlesPerRow - 1);
    for (int t = 0; t < m_settings.top.m_thickness; t++) {
        for (int i = 0; i < m_settings.top.m_nParticlesPerRow; i++) {
            m_boundaryparticles.emplace_back(m_settings.top.m_particleMass, Vec2d(particleDensity * i, m_settings.physicalSize.y + particleDensity*t),
                                             Vec2d(m_settings.top.m_velocity.x, m_settings.top.m_velocity.y));
        }
    }
    //left
    particleDensity = m_settings.physicalSize.y/(m_settings.left.m_nParticlesPerRow - 1);
    for (int t = 0; t < m_settings.left.m_thickness; t++) {
        for (int i = 0; i < m_settings.left.m_nParticlesPerRow; i++) {
            m_boundaryparticles.emplace_back(m_settings.left.m_particleMass, Vec2d(-particleDensity*t, particleDensity * i),
                                             Vec2d(m_settings.left.m_velocity.x, m_settings.left.m_velocity.y));
        }
    }
    //right
    particleDensity = m_settings.physicalSize.y/(m_settings.right.m_nParticlesPerRow - 1);
    for (int t = 0; t < m_settings.right.m_thickness; t++) {
        for (int i = 0; i < m_settings.right.m_nParticlesPerRow; i++) {
            m_boundaryparticles.emplace_back(m_settings.right.m_particleMass, Vec2d(m_settings.physicalSize.x + particleDensity*t, particleDensity * i),
                                             Vec2d(m_settings.right.m_velocity.x, m_settings.right.m_velocity.y));
        }
    }
}

void Simulation::run(OutputWriter& writer)
{
	writer.build_tree();
	auto out = m_particles;
	out.insert(out.end(), m_boundaryparticles.begin(), m_boundaryparticles.end());
	writer.write_vtp(out);

	double time = 0;
	double next_write = m_settings.vs_dt;
    bool firstIteration = true;
	refillGrid();
	while (time < m_settings.endTime)
	{
		calculateDensityAndPressure();
		calculateForces();
        leapfrog(firstIteration);
        firstIteration = false;
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
        p_i.forces = p_i.density*m_settings.g;
        for (const auto p_j : m_grid.neighbours(p_i.position, m_kernel->effectiveRadius()))
        {
            double vol_i = p_i.mass / p_i.density;
            double vol_j = p_j->mass / p_j->density;

            p_i.forces +=          -(p_i.pressure*vol_i + p_j->pressure*vol_j) / 2 * m_kernel->   gradW(p_i.position - p_j->position)
                    + m_settings.mu*(p_j->velocity*vol_j - p_i.velocity*vol_i) / 2 * m_kernel->laplaceW(p_i.position - p_j->position);
        }
	}
}

void Simulation::leapfrog(bool firstIteration) {
    const double dt_velocity = (0.5 + 0.5*(!firstIteration)) * m_settings.dt;
    for (auto& p : m_particles)
    {
        p.velocityAtHalfDt += dt_velocity * p.forces / p.density;
        p.position += m_settings.dt * p.velocityAtHalfDt;
        p.velocity = p.velocityAtHalfDt + m_settings.dt/2 * p.forces / p.density;
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
