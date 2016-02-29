//
//  viewer.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_VIEWER_HPP_
#define MFLUIDSOLVER_VIEWER_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../camera/camera.hpp"
#include "../scene/scene.hpp"

class Viewer {
public:
  Viewer() : Viewer(1024, 768){}
  Viewer(int width, int height);
  void run();

  Camera camera;
  Scene scene;

private:
  GLFWwindow *window;
};

#endif /* MFLUIDSOLVER_VIEWER_HPP_ */
