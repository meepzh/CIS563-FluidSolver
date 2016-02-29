//
//  main.cpp
//  MFluidSolver

#include <stdio.h>
#include "main.hpp"
#include "MFluidSolverConfig.hpp"

using namespace std;

int main() {
    fprintf(stdout,"Version %d.%d\n", MFluidSolver_VERSION_MAJOR, MFluidSolver_VERSION_MINOR);
    return 0;
}
