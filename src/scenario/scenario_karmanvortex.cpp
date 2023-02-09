#include "scenario_karmanvortex.h"
#include "datastructures/neighbourhood.h"
#include "datastructures/vector.h"
#include "simulation/simulation.h"

KarmanVortex::KarmanVortex(const Settings& settings, const std::shared_ptr<KernelFunction>& kernel)
	: Simulation(settings, kernel)
{
}

void KarmanVortex::initializeParticles()
{
	double area = m_settings.physicalSize.x * m_settings.physicalSize.y;
	nParticles.x = (int)std::lround(m_settings.physicalSize.x * sqrt(m_settings.nParticles / area));
	nParticles.y = m_settings.nParticles / nParticles.x;
	spacing.x = m_settings.physicalSize.x / (nParticles.x + 1);
	spacing.y = m_settings.physicalSize.y / (nParticles.y + 1);
	respawnTime = spacing.x / velocity;

	for (int x = 0; x < nParticles.x; x++)
	{
		for (int y = 0; y < nParticles.y; y++)
		{
			Vec2d position = Vec2d((x + 1) * spacing.x, (y + 1) * spacing.y);
			if (!insideObstacle(position))
				m_particles.emplace_back(m_settings.mass, position, Vec2d(velocity, 0));
			else
				m_boundaryParticles.emplace_back(m_settings.mass, position, Vec2d(0, 0));
		}
	}
}

void KarmanVortex::initializeBoundaries()
{
	double area = m_settings.physicalSize.x * m_settings.physicalSize.y;
	nParticles.x = (int)std::lround(m_settings.physicalSize.x * sqrt(m_settings.nParticles / area));
	nParticles.y = m_settings.nParticles / nParticles.x;
	spacing.x = m_settings.physicalSize.x / (nParticles.x + 1);
	spacing.y = m_settings.physicalSize.y / (nParticles.y + 1);

	// bottom
	for (int t = 0; t < m_settings.bottom.m_thickness; t++)
		for (int i = 1; i <= nParticles.x; i++)
			m_boundaryParticles.emplace_back(m_settings.bottom.m_particleMass, Vec2d(spacing.x * i, -spacing.y * t),
											 Vec2d(m_settings.bottom.m_velocity.x, m_settings.bottom.m_velocity.y));

	// top
	for (int t = 0; t < m_settings.top.m_thickness; t++)
		for (int i = 1; i <= nParticles.x; i++)
			m_boundaryParticles.emplace_back(m_settings.bottom.m_particleMass,
											 Vec2d(spacing.x * i, spacing.y * t + m_settings.physicalSize.y),
											 Vec2d(m_settings.top.m_velocity.x, m_settings.top.m_velocity.y));

	// left
	for (int t = 0; t < m_settings.left.m_thickness; t++)
		for (int i = 1 - m_settings.left.m_thickness; i <= nParticles.y + m_settings.top.m_thickness; i++)
			m_boundaryParticles.emplace_back(m_settings.left.m_particleMass, Vec2d(-spacing.x * t, spacing.y * i),
											 Vec2d(m_settings.left.m_velocity.x, m_settings.left.m_velocity.y));

	// right
	for (int t = 0; t < m_settings.right.m_thickness; t++)
		for (int i = 1 - m_settings.right.m_thickness; i <= nParticles.y + m_settings.top.m_thickness; i++)
			m_boundaryParticles.emplace_back(m_settings.right.m_particleMass,
											 Vec2d(spacing.x * t + m_settings.physicalSize.x, spacing.y * i),
											 Vec2d(m_settings.right.m_velocity.x, m_settings.right.m_velocity.y));
}

void KarmanVortex::calculateForces()
{
	for (auto it = m_particles.begin(); it != m_particles.end();)
	{
		// make all particles at the beginning free of force
		if (it->position.x > 0.6)
		{
			auto& p_i = *it;

			// apply gravity force
			p_i.forces = p_i.density * m_settings.g;

			// apply pressure and viscosity forces
			for (const auto& p_j : m_grid.neighbours(p_i.position, m_kernel->effectiveRadius()))
			{
				const double vol_i = p_i.mass / p_i.density;
				const double vol_j = p_j.mass / p_j.density;

				const Vec2d pressure =
					-0.5 * (p_i.pressure * vol_i + p_j.pressure * vol_j) * m_kernel->gradW(p_i.position - p_j.position);
				const Vec2d viscosity = (m_settings.mu * 0.5) * (p_j.velocity * vol_j - p_i.velocity * vol_i) *
										m_kernel->laplaceW(p_i.position - p_j.position);

				p_i.forces += pressure + viscosity;
			}
		}
		// despawn
		if (it->position.x > m_settings.physicalSize.x - 0.1)
		{
			it = m_particles.erase(it);
			continue; // skip ++it
		}
		++it;
	}
}

void KarmanVortex::update()
{
	// spawn new particles
	if (time > timeIt * respawnTime)
	{
		timeIt++;
		for (int y = 0; y < nParticles.y; y++)
		{
			Vec2d pos = Vec2d(spacing.x, (y + 1) * spacing.y);
			Particle p = Particle(m_settings.mass, pos, Vec2d(velocity, 0));
			m_particles.insert(m_particles.begin(), p);
		}
	}
}

bool KarmanVortex::insideObstacle(Vec2d pos)
{
	return euclideanDistance(pos, obstaclePos) <= obstacleRadius;
}
