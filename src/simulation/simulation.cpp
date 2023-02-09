#include <cassert>
#include <utility>
#include "simulation.h"
#include "datastructures/neighbourhood.h"
#include "scenario/scenario_liddrivencavity.h"

Simulation::Simulation(const Settings& settings, std::shared_ptr<KernelFunction> kernel) :
	m_kernel(std::move(kernel)),
	m_settings(settings),
	m_grid(settings)
    {}


void Simulation::initialize()
{
    initializeParticles();
    initializeBoundaries();
}

void Simulation::run(OutputWriter& writer)
{
    initialize();
    writer.build_tree();

    //output initial state
	writer.write_vtp(m_particles, m_boundaryParticles);

	time = 0;
	double next_write = m_settings.vs_dt;
    bool firstIteration = true;
	refillGrid();
	while (time < m_settings.endTime)
	{
		calculateDensityAndPressure();
		calculateForces();
        leapfrog(firstIteration);
        firstIteration = false;
        update();
		refillGrid();

		if (time >= next_write) {
			writer.write_vtp(m_particles, m_boundaryParticles);

			next_write += m_settings.vs_dt;
		}

		time += m_settings.dt;
		std::cout << "timestep t=" << time << "\n";
	}
}


Grid& Simulation::getGrid() {
	return m_grid;
}

void Simulation::calculateDensityAndPressure(Particle& particle)
{
	double rho = 0.0;

	for (const auto& neighbour_particle : m_grid.neighbours(particle.position, m_kernel->effectiveRadius()))
	{
		rho += neighbour_particle.mass * m_kernel->W(particle.position - neighbour_particle.position);
	}

	particle.density = rho;
	particle.pressure = m_settings.kappa * (pow(rho / m_settings.rho_0, 7) - 1);
}

void Simulation::calculateDensityAndPressure()
{
#pragma omp parallel for
	for (int i = 0; i < m_particles.size(); i++)
	{
        calculateDensityAndPressure(m_particles[i]);
	}

#pragma omp parallel for
	for (int i = 0; i < m_boundaryParticles.size(); i++)
	{
        calculateDensityAndPressure(m_boundaryParticles[i]);
	}
}


void Simulation::calculateForces()
{
#pragma omp parallel for
	for (int i = 0; i < m_particles.size(); i++)
	{
		auto& p_i = m_particles[i];

		// apply gravity force
		p_i.forces = p_i.density * m_settings.g;

		// apply pressure and viscosity forces
		for (const auto& p_j : m_grid.neighbours(p_i.position, m_kernel->effectiveRadius()))
		{
			const double vol_i = p_i.mass / p_i.density;
			const double vol_j = p_j.mass / p_j.density;

			const Vec2d pressure = -0.5 * (p_i.pressure * vol_i + p_j.pressure * vol_j) * m_kernel->gradW(p_i.position - p_j.position);
			const Vec2d viscosity = (m_settings.mu / 2.0) * (p_j.velocity * vol_j - p_i.velocity * vol_i) * m_kernel->laplaceW(p_i.position - p_j.position);

			p_i.forces += pressure + viscosity;
        }
	}
}

void Simulation::leapfrog(bool firstIteration) {
    const double dt_velocity = (0.5 + 0.5*(!firstIteration)) * m_settings.dt;
#pragma omp parallel for
    for (int i = 0; i < m_particles.size(); i++)
    {
        auto& p = m_particles[i];
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
	for (auto& p : m_boundaryParticles)
	{
		m_grid.add(&p);
	}
}

void Simulation::initializeParticles()
{
    Vec2i nParticles;
    double area = m_settings.physicalSize.x * m_settings.physicalSize.y;
    nParticles.x = (int)std::lround(m_settings.physicalSize.x * sqrt(m_settings.nParticles / area));
    nParticles.y = m_settings.nParticles / nParticles.x;
    Vec2d spacing(m_settings.physicalSize.x / (nParticles.x + 1), m_settings.physicalSize.y / (nParticles.y + 1));

    for (int y = 0; y < nParticles.y; y++)
    {
        for (int x = 0; x < nParticles.x; x++)
        {
            m_particles.emplace_back(m_settings.mass, Vec2d((x + 1) * spacing.x, (y + 1) * spacing.y), Vec2d(0, 0));
        }
    }
}

void Simulation::initializeBoundaries()
{
    Vec2i nParticles;
    double area = m_settings.physicalSize.x * m_settings.physicalSize.y;
    nParticles.x = (int)std::lround(m_settings.physicalSize.x * sqrt(m_settings.nParticles / area));
    nParticles.y = m_settings.nParticles / nParticles.x;
    Vec2d spacing(m_settings.physicalSize.x / (nParticles.x + 1), m_settings.physicalSize.y / (nParticles.y + 1));

    //bottom
    for (int t = 0; t < m_settings.bottom.m_thickness; t++)
        for (int i = 1; i <= nParticles.x; i++)
            m_boundaryParticles.emplace_back(m_settings.bottom.m_particleMass, Vec2d(spacing.x * i, -spacing.y * t), Vec2d(m_settings.bottom.m_velocity.x, m_settings.bottom.m_velocity.y));

    //top
    for (int t = 0; t < m_settings.top.m_thickness; t++)
        for (int i = 1; i <= nParticles.x; i++)
            m_boundaryParticles.emplace_back(m_settings.top.m_particleMass, Vec2d(spacing.x * i, spacing.y * t + m_settings.physicalSize.y), Vec2d(m_settings.top.m_velocity.x, m_settings.top.m_velocity.y));

    //left
    for (int t = 0; t < m_settings.left.m_thickness; t++)
        for (int i = 1 - m_settings.left.m_thickness; i <= nParticles.y + m_settings.top.m_thickness; i++)
            m_boundaryParticles.emplace_back(m_settings.left.m_particleMass, Vec2d(-spacing.x * t, spacing.y * i), Vec2d(m_settings.left.m_velocity.x, m_settings.left.m_velocity.y));

    //right
    for (int t = 0; t < m_settings.right.m_thickness; t++)
        for (int i = 1 - m_settings.right.m_thickness; i <= nParticles.y + m_settings.top.m_thickness; i++)
            m_boundaryParticles.emplace_back(m_settings.right.m_particleMass, Vec2d(spacing.x * t + m_settings.physicalSize.x, spacing.y * i), Vec2d(m_settings.right.m_velocity.x, m_settings.right.m_velocity.y));
}
