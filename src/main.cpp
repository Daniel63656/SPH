#include <iostream>
#include "datastructures/particle.h"
#include "datastructures/vec.h"
#include <vector>

class LinkedCell
{
public:
    void add(const Particle<Vec2>& p)
    {

    }
};

class Simulation
{
    Simulation()
    {
        particles.emplace_back();
    }

    void step(double dt)
    {
        //sort particles into cells
        for (auto& p : particles)
        {
            cells.add(p);
        }

        for (auto& p : particles)
        {
            //p.velocity = calculateVelocity();
        }
    }

private:
    std::vector<Particle<Vec2>> particles;
    LinkedCell cells;

};

int main() {

    //! read parameterFiles (these may be reassigned in UI)
    //! main file to start a simulation



    std::cout << "Hello, World!" << std::endl;
    return 0;
}
