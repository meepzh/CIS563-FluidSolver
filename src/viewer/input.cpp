//
//  input.cpp
//  MFluidSolver

#include "input.hpp"

Viewer *Input::viewer = nullptr;

void Input::computeArcballScrollCb(GLFWwindow* window, double xoffset, double yoffset) {
  if (viewer == nullptr) return;
  viewer->camera.zoom(yoffset);
}