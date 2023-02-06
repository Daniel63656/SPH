#include "scenario_karmanvortex.h"
#include "datastructures/vector.h"
#include "simulation/simulation.h"

void KarmanVortex::init(Simulation* sim)
{
    initializeParticles(sim);
    initializeBoundaries(sim);
}

void KarmanVortex::update(Simulation* sim)
{
}


void KarmanVortex::initializeParticles(Simulation* sim)
{
    Vec2i nParticles;
    double area = sim->m_settings.physicalSize.x * sim->m_settings.physicalSize.y;
    nParticles.x = (int)std::lround(sim->m_settings.physicalSize.x * sqrt(sim->m_settings.nParticles / area));
    nParticles.y = sim->m_settings.nParticles / nParticles.x;
    Vec2d spacing(sim->m_settings.physicalSize.x / (nParticles.x + 1), sim->m_settings.physicalSize.y / (nParticles.y + 1));

    for (int y = 0; y < nParticles.y; y++)
    {
        for (int x = 0; x < nParticles.x; x++)
        {
            sim->m_particles.emplace_back(sim->m_settings.mass, Vec2d((x + 1) * spacing.x, (y + 1) * spacing.y), Vec2d(0, 0));
        }
    }
}

void KarmanVortex::initializeBoundaries(Simulation* sim)
{
    Vec2i nParticles;
    double area = sim->m_settings.physicalSize.x * sim->m_settings.physicalSize.y;
    nParticles.x = (int)std::lround(sim->m_settings.physicalSize.x * sqrt(sim->m_settings.nParticles / area));
    nParticles.y = sim->m_settings.nParticles / nParticles.x;
    Vec2d spacing(sim->m_settings.physicalSize.x / (nParticles.x + 1), sim->m_settings.physicalSize.y / (nParticles.y + 1));

    //bottom
    for (int t = 0; t < sim->m_settings.bottom.m_thickness; t++)
        for (int i = 1; i <= nParticles.x; i++)
            sim->m_boundaryParticles.emplace_back(sim->m_settings.bottom.m_particleMass, Vec2d(spacing.x * i, -spacing.y * t), Vec2d(sim->m_settings.bottom.m_velocity.x, sim->m_settings.bottom.m_velocity.y));

    //top
    for (int t = 0; t < sim->m_settings.top.m_thickness; t++)
        for (int i = 1; i <= nParticles.x; i++)
            sim->m_boundaryParticles.emplace_back(sim->m_settings.bottom.m_particleMass, Vec2d(spacing.x * i, spacing.y * t + sim->m_settings.physicalSize.y), Vec2d(sim->m_settings.top.m_velocity.x, sim->m_settings.top.m_velocity.y));

    //left
    for (int t = 0; t < sim->m_settings.left.m_thickness; t++)
        for (int i = 1 - sim->m_settings.bottom.m_thickness; i <= nParticles.y + sim->m_settings.top.m_thickness; i++)
            sim->m_boundaryParticles.emplace_back(sim->m_settings.bottom.m_particleMass, Vec2d(-spacing.x * t, spacing.y * i), Vec2d(sim->m_settings.left.m_velocity.x, sim->m_settings.left.m_velocity.y));

    //right
    for (int t = 0; t < sim->m_settings.right.m_thickness; t++)
        for (int i = 1 - sim->m_settings.bottom.m_thickness; i <= nParticles.y + sim->m_settings.top.m_thickness; i++)
            sim->m_boundaryParticles.emplace_back(sim->m_settings.bottom.m_particleMass, Vec2d(spacing.x * t + sim->m_settings.physicalSize.x, spacing.y * i), Vec2d(sim->m_settings.right.m_velocity.x, sim->m_settings.right.m_velocity.y));
}