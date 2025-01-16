# Smoothed Particle Hydrodynamis Solver

This project implements the Smoothed Particle Hydrodynamis (SPH) approach in C++ using openMP for parallel execution.
Both Gaussian kernel and cubicSpline kernel are implemented as kernel functions. Simulation results are written
to be viewed with **ParaView**. 

## Getting Started

To run the simulations yourself, first clone the repository and navigate to it:
```bash
  git clone https://github.com/Daniel63656/SPH.git
  cd SPH
  ```

This project uses vcpkg to build the project with its dependencies (pugixml). To build the project, use your specific
vcpkg installation to build the project with all its dependencies.

Navigate to the project directory and run the publishToMavenLocal gradle task:
```bash
  cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=PATH_TO_YOUR_VCPKG_INSTALL/scripts/buildsystems/vcpkg.cmake
  cmake --build cmake-build-debug
  ```
Run a simulation:
```bash
  cmake-build-debug\Debug\SPH.exe karmanVortex.txt
 ```
