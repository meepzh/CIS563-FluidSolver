//  Copyright 2016 Robert Zhou
//
//  viewer.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_VIEWER_HPP_
#define MFLUIDSOLVER_VIEWER_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "MFluidSolverConfig.hpp"

#include "scene/scene.hpp"
#include "particleShaderProgram.hpp"
#include "utils.hpp"

struct GLFWWindowInitException : std::exception {
  const char *what() const noexcept {
    return "Failed to initialize GLFW window.\n";
  };
};
struct GLEWInitException : std::exception {
  const char *what() const noexcept {
    return "Failed to initialize GLEW.\n";
  };
};

class Viewer {
 public:
  Viewer();
  ~Viewer();
  void init(int width = MFluidSolver_DEFAULT_WINDOW_WIDTH,
            int height = MFluidSolver_DEFAULT_WINDOW_HEIGHT);
  void run();
  void screenshot(bool manual = false);
  void stop();
  void togglePause();

  void configureScreenshot(bool render, unsigned int skip);

  GLFWwindow *window;
  Scene scene;
  ShaderProgram *wireShader;
  ParticleShaderProgram *particleShader;

 private:
  int _width, _height;

  int leftState, rightState;
  int oldLeftState, oldRightState;
  glm::dvec2 oldPos, newPos;
  bool paused, shouldStop;

  bool autoRender;
  unsigned int renderSkip;
  std::vector<GLubyte> screenshotArray;
  std::vector<GLubyte> screenshotArrayFlipped;
  inline unsigned int screenshotIndex(unsigned int x, unsigned int y);
};

#endif  // MFLUIDSOLVER_VIEWER_HPP_
