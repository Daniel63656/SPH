#include <iostream>
#include "datastructures/vector.h"


int main() {

    //! read parameterFiles (these may be reassigned in UI)
    //! main file to start a simulation


    Vector<2> v = Vector<2>({1, 0});
    Vector<2> w = Vector<2>({1, 1});

    float f = euclideanDistance(v, w);

    std::cout << "dist: " << f << std::endl;
    return 0;
}
