//
//  main.cpp
//  MFluidSolver

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "MFluidSolverConfig.hpp"
#include "main.hpp"
#include "viewer/viewer.hpp"

int main() {
  fprintf(stdout,"Version %d.%d\n", MFluidSolver_VERSION_MAJOR, MFluidSolver_VERSION_MINOR);

  // Initialize GLFW
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initiaize GLFW.\n");
    return -1;
  }

  Viewer viewer;
  if (viewer.hasError()) {
    return -1;
  }
  viewer.run();

  return 0;
}
