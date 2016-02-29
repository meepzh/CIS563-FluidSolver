//
//  input.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_VIEWER_INPUT_HPP_
#define MFLUIDSOLVER_VIEWER_INPUT_HPP_

#include "viewer.hpp"

class Input {
public:
  static Viewer *viewer;
  static void computeArcballScrollCb(GLFWwindow* window, double xoffset, double yoffset);
};

#endif /* MFLUIDSOLVER_VIEWER_INPUT_HPP_ */