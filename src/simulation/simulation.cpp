#include <cassert>
#include "simulation.h"
#include "datastructures/neighbourhood.h"
#include "scenario/scenario_liddrivencavity.h"

Simulation::Simulation(const Settings& settings, const KernelFunction* kernel, MPI_Vars& mpi_info) :
	m_kernel(kernel),
	m_settings(settings),
	m_grid(settings),
	m_mpi_info(mpi_info)
{
    m_scenario = std::make_shared<LidDrivenCavity>();
    m_scenario->init(this);
	m_mpi_info.arrayend = m_particles.size() + m_boundaryparticles.size();
}

void Simulation::initializeParticles()
{

}

void Simulation::initializeBoundaries()
{

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
		m_scenario->update(this);
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

	for (const auto& neighbour_particle : m_grid.neighbours(particle.position, m_kernel->effectiveRadius()))
	{
		hasNeighbours = true;
		rho += neighbour_particle.mass * m_kernel->W(particle.position - neighbour_particle.position);
	}

	assert(hasNeighbours);
	particle.density = rho;
	particle.pressure = m_settings.kappa * (pow(rho / m_settings.rho_0, 7) - 1);
}

void Simulation::calculateDensityAndPressure() {

#pragma omp parallel for
	for (int i = 0; i < m_particles.size(); i++)
	{
		auto& p = m_particles[i];
		calcDensityPresure(p);
	}

#pragma omp parallel for
	for (int i = 0; i < m_boundaryparticles.size(); i++)
	{
		auto& p = m_boundaryparticles[i];
		calcDensityPresure(p);
	}
}


void Simulation::calculateForces()
{
#pragma omp parallel for
	for (int i = 0; i < m_particles.size(); i++)
	{
		auto& p_i = m_particles[i];
		p_i.forces = p_i.density * m_settings.g;
		for (const auto& p_j : m_grid.neighbours(p_i.position, m_kernel->effectiveRadius()))
		{
			double vol_i = p_i.mass / p_i.density;
			double vol_j = p_j.mass / p_j.density;
            p_i.forces +=          -(p_i.pressure*vol_i + p_j.pressure*vol_j) / 2 * m_kernel->   gradW(p_i.position - p_j.position)
                    + m_settings.mu*(p_j.velocity*vol_j - p_i.velocity*vol_i) / 2 * m_kernel->laplaceW(p_i.position - p_j.position);
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
	for (auto& p : m_boundaryparticles)
	{
		m_grid.add(&p);
	}
}
