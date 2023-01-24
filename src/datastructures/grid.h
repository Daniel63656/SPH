#pragma once

#include <iostream>
#include <vector>
#include "particle.h"

template<typename Vec>
class Grid
{
public:
    Grid();
    void clear();



private:
    int m_length;
    Vec dimensions;
    std::vector<std::vector<Particle<Vec>>> particles;

    int calcIndex(Vec vec);
};
