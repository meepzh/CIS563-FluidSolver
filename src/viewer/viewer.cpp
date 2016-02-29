//
//  viewer.cpp
//  MFluidSolver

#include "viewer.hpp"

#include <cstdio>

Viewer::Viewer(int width, int height)
: wireShader(nullptr) {
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
    throw -1;
  }

  glfwMakeContextCurrent(window); // Initialize GLEW

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::fprintf(stderr, "Failed to initialize GLEW.\n");
    glfwTerminate();
    throw -1;
  }

  // Ensure key capture
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

void Viewer::run() {
  do {
    glClear(GL_COLOR_BUFFER_BIT);

    wireShader->setViewProjectionMat(camera.getViewProjection());
    for (Geometry *g : scene.objects) {
      wireShader->setModelMat(g->transform.T());
      wireShader->draw(g);
    }

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  } while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);
}

Viewer::~Viewer() {
  delete wireShader;
}