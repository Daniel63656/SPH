#include <iostream>
#include "datastructures/vector.h"


int main() {

    //! read parameterFiles (these may be reassigned in UI)
    //! main file to start a simulation


    Vector<2> v = Vector<2>({1, 0});
    Vector<2> w = Vector<2>({0, 1});

    v = w;
    v *= 3;
    w[0] = 9;


    std::cout << "v: " << v << std::endl;
    std::cout << "w: " << w << std::endl;
    return 0;
}
