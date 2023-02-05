#pragma once


#include "particle.h"
#include "../settings/settings.h"
#include "pch.h"

class Neighbourhood;

class Grid
{
public:
    friend class Neighbourhood;

    Grid(const Settings& settings);

    void clear();
    void add(Particle* p);

    Neighbourhood neighbours(const Vec2d& center, double radius);

    //std::vector<Particle*> neighbours(const Vec2d& center, double radius) const;
private:
    const Settings& m_settings;

    int nTotal;                     //total number of cells
    Vec2d m_meshWidth{};            //width of the cells in all spacial directions
    std::vector<std::vector<Particle*>> grid;

    Vec2i discretizedPosition(Vec2d v) const;
    int pos2idx(Vec2i pos) const;
};

