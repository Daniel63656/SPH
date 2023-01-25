#pragma once

#include <iostream>
#include <vector>
#include "particle.h"

template <unsigned int N>
class Grid
{
public:
    Grid();
    void clear();



private:
    int m_length;
    Vector<N> dimensions;
    std::vector<std::vector<Particle<N>>> particles;

    int calcIndex(Vector<N> v);
};
