#include "grid.h"

template<typename V>
int Grid<V>::calcIndex(V vec) {
    return 0;
}

template<typename V>
void Grid<V>::clear()
{
    for (int i = 0; i < m_length; i++)
        particles[i].clear();
}