//
//  input.cpp
//  MFluidSolver

#include "input.hpp"

Viewer *Input::viewer = nullptr;

void Input::computeArcballScrollCb(GLFWwindow* window, double xoffset, double yoffset) {
  if (viewer == nullptr) return;
  viewer->camera.zoom(yoffset);
}

void Input::checkKeys(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (viewer == nullptr) return;
  if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
    viewer->togglePause();
  } else if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
    viewer->stop();
  }
}