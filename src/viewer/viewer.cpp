//
//  viewer.cpp
//  MFluidSolver

#include "viewer.hpp"

#include <cstdio>
#include "input.hpp"

Viewer::Viewer(int width, int height)
: wireShader(nullptr), particleShader(nullptr),
  oldLeftState(GLFW_RELEASE), oldRightState(GLFW_RELEASE) {
  // http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

  window = glfwCreateWindow(width, height, "MFluidSolver", NULL, NULL);
  if (window == NULL) {
    std::fprintf(stderr, "Failed to open GLFW window\n");
    getchar(); // Wait for key before quit
    glfwTerminate();
    throw -1;
  }

  glfwMakeContextCurrent(window); // Initialize GLEW

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::fprintf(stderr, "Failed to initialize GLEW\n");
    getchar(); // Wait for key before quit
    glfwTerminate();
    throw -1;
  }

  // Sticky keys and mouse buttons
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

  // Mouse callbacks
  glfwSetScrollCallback(window, Input::computeArcballScrollCb);
}

void Viewer::run() {
  double oldTime = glfwGetTime();
  double currTime, deltaT;
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Get elapsed time
    currTime = glfwGetTime();
    deltaT = currTime - oldTime;
    oldTime = currTime;

    // Update and render particles
    solver.update(deltaT);

    // Render boxes
    wireShader->setViewProjectionMat(camera.getViewProjection());
    for (Geometry *g : scene.objects) {
      wireShader->setModelMat(g->transform.T());
      wireShader->draw(g);
    }

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

    // Parse mouse button input
    leftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    rightState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (leftState == GLFW_PRESS) {
      if (oldLeftState == GLFW_RELEASE) {
        glfwGetCursorPos(window, &oldPos.x, &oldPos.y);
      } else {
        glfwGetCursorPos(window, &newPos.x, &newPos.y);
        camera.arcball(oldPos, newPos);
        oldPos = glm::vec2(newPos);
      }
    }
    if (rightState == GLFW_PRESS) {
      if (oldRightState == GLFW_RELEASE) {
        glfwGetCursorPos(window, &oldPos.x, &oldPos.y);
      } else {
        glfwGetCursorPos(window, &newPos.x, &newPos.y);
        camera.pan(oldPos, newPos);
        oldPos = glm::vec2(newPos);
      }
    }
    oldLeftState = leftState;
    oldRightState = rightState;
  } while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);
}

Viewer::~Viewer() {
  delete wireShader;
}
