//
//  viewer.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_VIEWER_HPP_
#define MFLUIDSOLVER_VIEWER_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Viewer {
public:
  Viewer() : Viewer(1024, 768){}
  Viewer(int width, int height);
  bool hasError();
  void run();
private:
  GLFWwindow *window;
  bool _hasError;
};

#endif /* MFLUIDSOLVER_VIEWER_HPP_ */
