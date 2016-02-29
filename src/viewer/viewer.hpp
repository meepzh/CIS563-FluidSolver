//
//  viewer.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_VIEWER_HPP_
#define MFLUIDSOLVER_VIEWER_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../camera/camera.hpp"
#include "../fluidSolver/fluidSolver.hpp"
#include "../scene/scene.hpp"
#include "particleShaderProgram.hpp"

class Viewer {
public:
  Viewer() : Viewer(1024, 768){}
  Viewer(int width, int height);
  ~Viewer();
  void run();

  Camera camera;
  FluidSolver solver;
  GLFWwindow *window;
  Scene scene;
  ShaderProgram *wireShader;
  ParticleShaderProgram *particleShader;

private:
  int leftState, rightState;
  int oldLeftState, oldRightState;
  glm::dvec2 oldPos, newPos;
};

#endif /* MFLUIDSOLVER_VIEWER_HPP_ */
