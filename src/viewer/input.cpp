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
  if (action == GLFW_RELEASE) {
    switch (key) {
      case GLFW_KEY_ESCAPE:
        viewer->stop();
        break;
      case GLFW_KEY_E:
        #if MFluidSolver_USE_OPENVDB
        viewer->scene.solver.exportVDB();
        #endif
        break;
      case GLFW_KEY_N:
        viewer->scene.solver.visualizeRandomParticlesNeighbors();
        break;
      case GLFW_KEY_P:
        viewer->togglePause();
        break;
      case GLFW_KEY_R:
        viewer->scene.seedScene();
        break;
      case GLFW_KEY_S:
        viewer->screenshot(true);
        break;
      case GLFW_KEY_W:
        #if MFluidSolver_USE_PARTIO
        viewer->scene.solver.exportBgeo();
        #endif
        break;
      case GLFW_KEY_RIGHT:
        viewer->scene.solver.updateStep();
        break;
    }
  }
}