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
    //despawn
    for (auto it = sim->m_particles.begin(); it != sim->m_particles.end(); )
    {

            it->forces.x = 0;
            it->forces.y = 0;
            ++it;
            continue;



        if (it->position.x > 9.8)
            it = sim->m_particles.erase(it);
        else
            ++it;
    }




    /*for (int i = 0; i < sim->m_particles.size(); i++)
    {
        auto& p_i = sim->m_particles[i];
        if (p_i.position.x > 10)
        {
            p_i.velocity.x = 1;
            p_i.velocityAtHalfDt.x = 1;
            p_i.velocity.y = 0;
            p_i.velocityAtHalfDt.y = 0;
            p_i.position.x -= 10;
        }
    }*/

/*    //spawn
    for (int y = 0; y < nParticles.y; y++)
    {
        sim->m_particles.emplace_back(sim->m_settings.mass, Vec2d(spacing.x, (y + 1) * spacing.y) , Vec2d(1, 0));
    }*/
}


void KarmanVortex::initializeParticles(Simulation* sim)
{
    double area = sim->m_settings.physicalSize.x * sim->m_settings.physicalSize.y;
    nParticles.x = (int)std::lround(sim->m_settings.physicalSize.x * sqrt(sim->m_settings.nParticles / area));
    nParticles.y = sim->m_settings.nParticles / nParticles.x;
    spacing.x = sim->m_settings.physicalSize.x / (nParticles.x + 1);
    spacing.y = sim->m_settings.physicalSize.y / (nParticles.y + 1);

    for (int y = 0; y < nParticles.y; y++)
    {
        for (int x = 0; x < nParticles.x; x++)
        {
            Vec2d position = Vec2d((x + 1) * spacing.x, (y + 1) * spacing.y);
            if (!insideObstacle(position))
                sim->m_particles.emplace_back(sim->m_settings.mass, position , Vec2d(1, 0));
            else
                sim->m_boundaryParticles.emplace_back(sim->m_settings.mass, position , Vec2d(0, 0));
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
            sim->m_boundaryParticles.emplace_back(sim->m_settings.bottom.m_particleMass, Vec2d(spacing.x * i, -spacing.y * t), Vec2d((spacing.x * i < 1)*sim->m_settings.bottom.m_velocity.x, sim->m_settings.bottom.m_velocity.y));

    //top
    for (int t = 0; t < sim->m_settings.top.m_thickness; t++)
        for (int i = 1; i <= nParticles.x; i++)
            sim->m_boundaryParticles.emplace_back(sim->m_settings.bottom.m_particleMass, Vec2d(spacing.x * i, spacing.y * t + sim->m_settings.physicalSize.y), Vec2d((spacing.x * i < 1)*sim->m_settings.top.m_velocity.x, sim->m_settings.top.m_velocity.y));

    //left
    for (int t = 0; t < sim->m_settings.left.m_thickness; t++)
        for (int i = 1 - sim->m_settings.left.m_thickness; i <= nParticles.y + sim->m_settings.top.m_thickness; i++)
            sim->m_boundaryParticles.emplace_back(sim->m_settings.left.m_particleMass, Vec2d(-spacing.x * t, spacing.y * i), Vec2d(sim->m_settings.left.m_velocity.x, sim->m_settings.left.m_velocity.y));

    //right
    for (int t = 0; t < sim->m_settings.right.m_thickness; t++)
        for (int i = 1 - sim->m_settings.right.m_thickness; i <= nParticles.y + sim->m_settings.top.m_thickness; i++)
            sim->m_boundaryParticles.emplace_back(sim->m_settings.right.m_particleMass, Vec2d(spacing.x * t + sim->m_settings.physicalSize.x, spacing.y * i), Vec2d(sim->m_settings.right.m_velocity.x, sim->m_settings.right.m_velocity.y));
}

bool KarmanVortex::insideObstacle(Vec2d pos) {
    return euclideanDistance(pos, obstaclePos) <= obstacleRadius;
}
