//
//  viewer.cpp
//  MFluidSolver

#include "viewer.hpp"

#include <cstdio>
#include <cstring>

#include "input.hpp"

#if MFluidSolver_OPENGL_DEBUG
void APIENTRY openglDebugCallbackFunction(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, void* userParam) {
  std::string sType;
  std::string sSeverity;
  FILE **outStream = &stdout;

  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      sType = "ERROR";
      outStream = &stderr;
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      sType = "DEPRECATED";
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      sType = "UNDEFINED";
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      sType = "PORTABILITY";
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      sType = "PERFORMANCE";
      break;
    case GL_DEBUG_TYPE_OTHER:
      sType = "OTHER";
      break;
  }
  switch (severity){
    case GL_DEBUG_SEVERITY_LOW:
      sSeverity = "LOW";
      #if MFluidSolver_LOG_LEVEL > MFluidSolver_LOG_WARN
      return;
      #endif
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      sSeverity = "MEDIUM";
      #if MFluidSolver_LOG_LEVEL > MFluidSolver_LOG_ERROR
      return;
      #endif
      break;
    case GL_DEBUG_SEVERITY_HIGH:
      sSeverity = "HIGH";
      #if MFluidSolver_LOG_LEVEL > MFluidSolver_LOG_FATAL
      return;
      #endif
      break;
    default:
      sSeverity = "OTHER";
      #if MFluidSolver_LOG_LEVEL > MFluidSolver_LOG_INFO
      return;
      #endif
  }
  std::fprintf(*outStream, "GL:%s(%s): %s\n", sType.c_str(), sSeverity.c_str(), message);
}
#endif

Viewer::Viewer(int width, int height)
: wireShader(nullptr), particleShader(nullptr),
  oldLeftState(GLFW_RELEASE), oldRightState(GLFW_RELEASE),
  paused(true), shouldStop(false) {
  // http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

  #if MFluidSolver_OPENGL_DEBUG
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  #endif

  window = glfwCreateWindow(width, height, "MFluidSolver", NULL, NULL);
  if (window == NULL) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_FATAL
    std::fprintf(stderr, "FATAL: Failed to open GLFW window\n");
    #endif

    getchar(); // Wait for key before quit
    glfwTerminate();
    throw -1;
  }

  glfwMakeContextCurrent(window); // Initialize GLEW

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_FATAL
    std::fprintf(stderr, "FATAL: Failed to initialize GLEW\n");
    #endif

    getchar(); // Wait for key before quit
    glfwTerminate();
    throw -1;
  }

  // Sticky keys and mouse buttons
  glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

  // Key callback
  glfwSetKeyCallback(window, Input::checkKeys);

  // Mouse callbacks
  glfwSetScrollCallback(window, Input::computeArcballScrollCb);

  #if MFluidSolver_OPENGL_DEBUG
  // Set debug callback
  if (glDebugMessageCallback) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
    std::printf("DEBUG: Registering OpenGL debug callback\n");
    #endif

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglDebugCallbackFunction, nullptr);
    GLuint unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH,
      0, &unusedIds, true);
  } else {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
    std::printf("DEBUG: OpenGL debug callback not available\n");
    #endif
  }
  #endif
}

void Viewer::run() {
  double oldTime = glfwGetTime();
  double currTime, deltaT;
  GLenum glErrorCode;
  const GLubyte *glErrorString;

  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Get elapsed time
    currTime = glfwGetTime();
    deltaT = currTime - oldTime;
    oldTime = currTime;

    // Update and render particles
    if (!paused) {
      scene.solver.update(deltaT);
    }

    // Render particles
    particleShader->setViewProjectionMat(camera.getViewProjection());
    particleShader->setCameraVectors(camera.right(), camera.up());
    particleShader->draw();

    // Render boxes
    wireShader->setViewProjectionMat(camera.getViewProjection());
    for (Geometry *g : scene.objects) {
      wireShader->setModelMat(g->transform.T());
      wireShader->draw(g);
    }

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

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

    #if MFluidSolver_OPENGL_DEBUG == 0
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_ERROR
    // Check for errors
    if ((glErrorCode = glGetError()) != GL_NO_ERROR) {
      glErrorString = gluErrorString(glErrorCode);
      std::fprintf(stderr, "GL:ERROR: %s\n", glErrorString);
    }
    #endif
    #endif
  } while (!shouldStop &&
           glfwWindowShouldClose(window) == 0);
}

void Viewer::togglePause() {
  paused = !paused;
}

void Viewer::stop() {
  shouldStop = true;
}

Viewer::~Viewer() {
  delete wireShader;
}