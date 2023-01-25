#include "grid.h"


template<unsigned int N>
Grid<N>::Grid()
{
    nCells = 1;
    for (int i = 0; i< N ; i++)
    {
        meshWidth[i] = physicalSize[i] / size[i];
        nCells *= size[i];
    }

    grid.reserve(nCells);
    for (int i = 0; i < nCells; i++)
        grid.push_back(std::vector<Particle<N>>());
}


template<unsigned int N>
void Grid<N>::clear()
{
    for (int i = 0; i < nCells; i++)
        grid[i].clear();
}

template<unsigned int N>
void Grid<N>::add(Particle<N> p) {
    calc_pos(p.position);

}

template<unsigned int N>
void Grid<N>::remove(Particle<N> p) {

}

template<unsigned int N>
int Grid<N>::pos2idx(Vector<N> pos)
{
    std::array<int, N> discretePos;
    for (int i = 0; i< N ; i++)
    {
        discretePos[i] = int(pos.x/meshWidth[i]);

        //TODO limiting necessary?
    }
    //TODO combine to index
    return 0;
}

template<unsigned int N>
std::vector<Particle<N>> Grid<N>::neighbours(Particle<N> center, float radius) {
    std::vector<Particle<N>> neighbours;



    return neighbours;
}
