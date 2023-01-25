#include "grid.h"


template<unsigned int N>
void Grid<N>::clear()
{
    for (int i = 0; i < m_length; i++)
        particles[i].clear();
}

template<unsigned int N>
int Grid<N>::calcIndex(Vector<N> v) {
    return 0;
}