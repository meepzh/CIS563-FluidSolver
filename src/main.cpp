//
//  main.cpp
//  MFluidSolver

#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
// #include <openvdb/openvdb.h>

#include "MFluidSolverConfig.hpp"
#include "main.hpp"
#include "viewer/input.hpp"
#include "viewer/particleShaderProgram.hpp"

int main() {
  // Print version info
  printf("INFO: Version %d.%d\n", MFluidSolver_VERSION_MAJOR, MFluidSolver_VERSION_MINOR);

  // Initialize GLFW
  if (!glfwInit()) {
    fprintf(stderr, "ERROR: Failed to initiaize GLFW\n");
    getchar(); // Wait for key before quit
    return -1;
  }

  std::string sceneJSON = "scene/scene.json";
  std::string wireVShader = "glsl/wire.vert.glsl";
  std::string wireFShader = "glsl/wire.frag.glsl";
  std::string particleVShader = "glsl/particle.vert.glsl";
  std::string particleFShader = "glsl/particle.frag.glsl";
  std::string particleTexture = "texture/particle.dds";

  Viewer viewer;
  Input::viewer = &viewer;
  printf("INFO: OpenGL Version %s\n", glGetString(GL_VERSION));

  // Set a few settings/modes in OpenGL rendering
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

  // Set the size with which points should be rendered
  glPointSize(5);

  // Set background color
  glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

  // Bind vertex array object
  GLuint vaoID;
  glGenVertexArrays(1, &vaoID);
  glBindVertexArray(vaoID);

  // After vao init
  viewer.wireShader = new ShaderProgram(wireVShader, wireFShader);
  viewer.particleShader = new ParticleShaderProgram(&(viewer.scene.solver), particleVShader, particleFShader, particleTexture);
  viewer.scene.loadJSON(sceneJSON);

  viewer.run();

  // Cleanup
  glDeleteVertexArrays(1, &vaoID);

  return 0;
}
