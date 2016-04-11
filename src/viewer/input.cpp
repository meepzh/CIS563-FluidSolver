//
//  input.cpp
//  MFluidSolver

#include "input.hpp"

Viewer *Input::viewer = nullptr;

void Input::computeArcballScrollCb(GLFWwindow* window, double xoffset, double yoffset) {
  if (viewer == nullptr) return;
  viewer->scene.camera.zoom(yoffset);
}

void Input::checkKeys(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (viewer == nullptr) return;
  if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
    viewer->togglePause();
  } else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
    viewer->screenshot(true);
  } else if (key == GLFW_KEY_R && action == GLFW_RELEASE) {
    viewer->scene.seedScene();
  } else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
    viewer->scene.solver.updateStep();
  } else if (key == GLFW_KEY_N && action == GLFW_RELEASE) {
    viewer->scene.solver.visualizeRandomParticlesNeighbors();
  } else if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
    #if MFluidSolver_USE_OPENVDB
    viewer->scene.solver.exportVDB();
    #endif
  } else if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
    viewer->stop();
  }
}