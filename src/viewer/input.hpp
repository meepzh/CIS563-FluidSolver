//
//  input.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_VIEWER_INPUT_HPP_
#define MFLUIDSOLVER_VIEWER_INPUT_HPP_

#include "MFluidSolverConfig.hpp"

#include "viewer.hpp"

class Input {
public:
  static Viewer *viewer;
  static void computeArcballScrollCb(GLFWwindow* window, double xoffset, double yoffset);
  static void checkKeys(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif /* MFLUIDSOLVER_VIEWER_INPUT_HPP_ */