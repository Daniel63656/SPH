#include "scenario_karmanvortex.h"
#include "datastructures/vector.h"
#include "simulation/simulation.h"


KarmanVortex::KarmanVortex(const Settings &settings, const std::shared_ptr<KernelFunction> &kernel, MPI_Vars &mpiInfo)
        : Simulation(settings, kernel, mpiInfo) {}


void KarmanVortex::initialize()
{
    initializeParticles();
    initializeBoundaries();
}

void KarmanVortex::initializeParticles()
{
    double area = m_settings.physicalSize.x * m_settings.physicalSize.y;
    nParticles.x = (int)std::lround(m_settings.physicalSize.x * sqrt(m_settings.nParticles / area));
    nParticles.y = m_settings.nParticles / nParticles.x;
    spacing.x = m_settings.physicalSize.x / (nParticles.x + 1);
    spacing.y = m_settings.physicalSize.y / (nParticles.y + 1);

    for (int y = 0; y < nParticles.y; y++)
    {
        for (int x = 0; x < nParticles.x; x++)
        {
            Vec2d position = Vec2d((x + 1) * spacing.x, (y + 1) * spacing.y);
            if (!insideObstacle(position))
                m_particles.emplace_back(m_settings.mass, position , Vec2d(1, 0));
            else
                m_boundaryParticles.emplace_back(m_settings.mass, position , Vec2d(0, 0));
        }
    }
}

void KarmanVortex::initializeBoundaries()
{
    Vec2i nParticles;
    double area = m_settings.physicalSize.x * m_settings.physicalSize.y;
    nParticles.x = (int)std::lround(m_settings.physicalSize.x * sqrt(m_settings.nParticles / area));
    nParticles.y = m_settings.nParticles / nParticles.x;
    Vec2d spacing(m_settings.physicalSize.x / (nParticles.x + 1), m_settings.physicalSize.y / (nParticles.y + 1));

    //bottom
    for (int t = 0; t < m_settings.bottom.m_thickness; t++)
        for (int i = 1; i <= nParticles.x; i++)
            m_boundaryParticles.emplace_back(m_settings.bottom.m_particleMass, Vec2d(spacing.x * i, -spacing.y * t), Vec2d((spacing.x * i < 1)*m_settings.bottom.m_velocity.x, m_settings.bottom.m_velocity.y));

    //top
    for (int t = 0; t < m_settings.top.m_thickness; t++)
        for (int i = 1; i <= nParticles.x; i++)
            m_boundaryParticles.emplace_back(m_settings.bottom.m_particleMass, Vec2d(spacing.x * i, spacing.y * t + m_settings.physicalSize.y), Vec2d((spacing.x * i < 1)*m_settings.top.m_velocity.x, m_settings.top.m_velocity.y));

    //left
    for (int t = 0; t < m_settings.left.m_thickness; t++)
        for (int i = 1 - m_settings.left.m_thickness; i <= nParticles.y + m_settings.top.m_thickness; i++)
            m_boundaryParticles.emplace_back(m_settings.left.m_particleMass, Vec2d(-spacing.x * t, spacing.y * i), Vec2d(m_settings.left.m_velocity.x, m_settings.left.m_velocity.y));

    //right
    for (int t = 0; t < m_settings.right.m_thickness; t++)
        for (int i = 1 - m_settings.right.m_thickness; i <= nParticles.y + m_settings.top.m_thickness; i++)
            m_boundaryParticles.emplace_back(m_settings.right.m_particleMass, Vec2d(spacing.x * t + m_settings.physicalSize.x, spacing.y * i), Vec2d(m_settings.right.m_velocity.x, m_settings.right.m_velocity.y));
}

void KarmanVortex::calculateForces() {
    //despawn
    for (auto it = m_particles.begin(); it != m_particles.end(); )
    {

        it->forces.x = 0;
        it->forces.y = 0;
        ++it;
        continue;



        if (it->position.x > 9.8)
            it = m_particles.erase(it);
        else
            ++it;
    }




    /*for (int i = 0; i < m_particles.size(); i++)
    {
        auto& p_i = m_particles[i];
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
        m_particles.emplace_back(m_settings.mass, Vec2d(spacing.x, (y + 1) * spacing.y) , Vec2d(1, 0));
    }*/
}


bool KarmanVortex::insideObstacle(Vec2d pos) {
    return euclideanDistance(pos, obstaclePos) <= obstacleRadius;
}