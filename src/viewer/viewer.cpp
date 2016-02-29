//
//  viewer.cpp
//  MFluidSolver

#include "viewer.hpp"

#include <cstdio>

Viewer::Viewer(int width, int height) : _hasError(false) {
  // http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

  window = glfwCreateWindow(width, height, "MFluidSolver", NULL, NULL);
  if (window == NULL) {
    std::fprintf(stderr, "Failed to open GLFW window.\n");
    glfwTerminate();
    _hasError = true;
    return;
  }

  glfwMakeContextCurrent(window); // Initialize GLEW

  if (glewInit() != GLEW_OK) {
    std::fprintf(stderr, "Failed to initialize GLEW.\n");
    glfwTerminate();
    _hasError = true;
    return;
  }

  // Ensure key capture
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

void Viewer::run() {
  do {
    glClear(GL_COLOR_BUFFER_BIT);

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  } while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);
}

bool Viewer::hasError() {
  return _hasError;
}