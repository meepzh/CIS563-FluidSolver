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
#include "viewer/shaderProgram.hpp"

int main() {
  std::fprintf(stdout,"Version %d.%d\n", MFluidSolver_VERSION_MAJOR, MFluidSolver_VERSION_MINOR);

  // Initialize GLFW
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initiaize GLFW.\n");
    return -1;
  }

  Viewer viewer;
  viewer.scene.loadJSON("scene.json");
  viewer.wireShader = new ShaderProgram("wire.vert.glsl", "wire.frag.glsl");

  // Set background color
  glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

  // Bind vertex array object
  GLuint vaoID;
  glGenVertexArrays(1, &vaoID);
  glBindVertexArray(vaoID);

  viewer.run();

  // Cleanup
  glDeleteVertexArrays(1, &vaoID);

  return 0;
}
