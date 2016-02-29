//
//  main.cpp
//  MFluidSolver

#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "MFluidSolverConfig.hpp"
#include "main.hpp"
#include "viewer/viewer.hpp"

int main() {
  std::fprintf(stdout,"Version %d.%d\n", MFluidSolver_VERSION_MAJOR, MFluidSolver_VERSION_MINOR);

  // Initialize GLFW
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initiaize GLFW.\n");
    return -1;
  }

  Viewer viewer;
  if (viewer.hasError()) {
    return -1;
  }

  // Set background color
  glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

  viewer.run();

  return 0;
}
