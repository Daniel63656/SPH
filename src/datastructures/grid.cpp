#include "grid.h"



template<unsigned int N>
void Grid<N>::clear()
{
    for (int i = 0; i < nCells; i++)
        grid[i].clear();
}

template<unsigned int N>
void Grid<N>::add(Particle<N> p) {
    int idx = pos2idx(discretizedPosition(p.position));
    grid[idx].push_back(p);
}

//! make nested search loop over N dimensions possible with a recursive function
//! @param dim original dimension that gets reduced by one in each recursive call
//! @param start array holding N start positions for the loop indices
//! @param end array holding N end positions for the loop indices
//! @param idx pointer to an int array holding the looping indices (e.g. i,j,k)
//! @param center Particle the neighbours search is centered at
//! @param radius search radius
//! @param neighbours collection to save found neighbours to

template<unsigned int N>
void Grid<N>::searchDimension(int dim, std::array<int, N> start, std::array<int, N> end, int* idx,
                              const Particle<N> center, double radius, std::vector<Particle<N>>& neighbours) {
    //all dimensions done (dim = 0)
    if(!dim) {
        for (Particle<N> p : grid[pos2idx(idx)])
        {
            if (euclideanDistance(p.position, center.position) <= radius)
            {
                neighbours.push_back(p);
            }
        }
        return;
    }

    for(int i = start[dim]; i < end[dim]; i++)
    {
        idx[N - dim] = i;
        loop(dim - 1, start, end, idx);
    }
}

template<unsigned int N>
std::vector<Particle<N>> Grid<N>::neighbours(Particle<N> center, double radius) {
    std::vector<Particle<N>> neighbours;
    std::array<int, N> start = discretizedPosition(center.position -= radius);
    std::array<int, N> end   = discretizedPosition(center.position += radius);

    int idx[N];
    searchDimension(N, start, end, idx, neighbours);
    return neighbours;
}

template<unsigned int N>
std::array<int, N> Grid<N>::discretizedPosition(Vector<N> v) {
    std::array<int, N> discretePos;
    for (int i = 0; i < N ; i++)
    {
        discretePos[i] = int(v[i]/meshWidth[i]);
        //TODO limiting necessary?
    }
    return discretePos;
}

//! map N dimensional index-vector into a unique, seamless scalar index
template<unsigned int N>
int Grid<N>::pos2idx(std::array<int, N> pos) {
    int idx = pos[0];
    int factor = 1;
    for (int i = 1; i < pos.size(); i++)
    {
        factor *= size[i-1];
        idx += factor*pos[i];
    }
    return idx;
}
