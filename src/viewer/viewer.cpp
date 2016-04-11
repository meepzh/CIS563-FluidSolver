//
//  viewer.cpp
//  MFluidSolver

#include "viewer.hpp"

#include <algorithm>
#include <boost/filesystem.hpp>
#include <cstring>
#include <ctime>
#include <stb_image/stb_image_write.h>
#include <string>

#include "input.hpp"

#if MFluidSolver_USE_TBB
#include <tbb/blocked_range2d.h>
#include <tbb/parallel_for.h>
#endif

#if MFluidSolver_OPENGL_DEBUG
void APIENTRY openglDebugCallbackFunction(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, void* userParam) {
  std::string sType;
  std::string sSeverity;
  std::ostream *outStream = &std::cout;

  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      sType = "ERROR";
      outStream = &std::cerr;
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
      #if MFluidSolver_LOG_LEVEL > MFluidSolver_LOG_ERROR
      return;
      #endif
      break;
    default:
      sSeverity = "OTHER";
      #if MFluidSolver_LOG_LEVEL > MFluidSolver_LOG_INFO
      return;
      #endif
  }
  *outStream << "GL:" << sType << "(" << sSeverity << "): " << message << std::endl;
}
#endif

Viewer::Viewer()
: wireShader(nullptr), particleShader(nullptr),
  oldLeftState(GLFW_RELEASE), oldRightState(GLFW_RELEASE),
  paused(true), shouldStop(false),
  autoRender(MFluidSolver_DEFAULT_AUTORENDER), renderSkip(MFluidSolver_DEFAULT_RENDERSKIP) {
}

Viewer::~Viewer() {
  delete wireShader;
}

void Viewer::init(int width, int height) {
  _width = width;
  _height = height;

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
    std::cerr << "FATAL: Failed to open GLFW window" << std::endl;
    #endif

    getchar(); // Wait for key before quit
    glfwTerminate();
    throw GLFWWindowInitException();
  }

  glfwMakeContextCurrent(window); // Initialize GLEW

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_FATAL
    std::cerr << "FATAL: Failed to initialize GLEW" << std::endl;
    #endif

    getchar(); // Wait for key before quit
    glfwTerminate();
    throw GLEWInitException();
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
    std::cout << "DEBUG: Registering OpenGL debug callback" << std:: endl;
    #endif

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openglDebugCallbackFunction, nullptr);
    GLuint unusedIds = 0;
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH,
      0, &unusedIds, true);
  } else {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
    std::cout << "DEBUG: OpenGL debug callback not available" << std::endl;
    #endif
  }
  #endif

  // Init screenshot array
  screenshotArray = std::vector<GLubyte>(3 * width * height);
  screenshotArrayFlipped = std::vector<GLubyte>(3 * width * height);

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  if (paused) std::cout << "INFO: We are currently paused!" << std::endl;
  if (!paused) std::cout << "INFO: We are currently playing!" << std::endl;
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
      if (scene.solver.hasEndedSimulation()) {
        togglePause();
      }
    }

    // Render particles
    particleShader->setViewProjectionMat(scene.camera.getViewProjection());
    particleShader->setCameraVectors(scene.camera.right(), scene.camera.up());
    particleShader->draw();

    // Render boxes
    wireShader->setViewProjectionMat(scene.camera.getViewProjection());
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
        scene.camera.arcball(oldPos, newPos);
        oldPos = glm::vec2(newPos);
      }
    }
    if (rightState == GLFW_PRESS) {
      if (oldRightState == GLFW_RELEASE) {
        glfwGetCursorPos(window, &oldPos.x, &oldPos.y);
      } else {
        glfwGetCursorPos(window, &newPos.x, &newPos.y);
        scene.camera.pan(oldPos, newPos);
        oldPos = glm::vec2(newPos);
      }
    }
    oldLeftState = leftState;
    oldRightState = rightState;

    // Swap buffers
    glfwSwapBuffers(window);

    // Render
    if (autoRender) {
      if (renderSkip > 1 && scene.solver.updateNumber() % renderSkip == 0) {
        screenshot(false);
      }
    }

    glfwPollEvents();

    #if MFluidSolver_OPENGL_DEBUG == 0
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_ERROR
    // Check for errors
    if ((glErrorCode = glGetError()) != GL_NO_ERROR) {
      glErrorString = gluErrorString(glErrorCode);
      std::cerr << "GL:ERROR: " << glErrorString << std::endl;
    }
    #endif
    #endif
  } while (!shouldStop &&
           glfwWindowShouldClose(window) == 0);
}

inline unsigned int Viewer::screenshotIndex(unsigned int x, unsigned int y) {
  return y * _width + x;
}

void Viewer::screenshot(bool manual) {
  // Capture
  glReadPixels(0, 0, _width, _height, GL_RGB, GL_UNSIGNED_BYTE, screenshotArray.data());

  // Flip
  std::string outString = MUtils::zeroPad(scene.solver.updateNumber(), 6) + ".tga";
  if (manual) {
    outString = "screenshot_" + outString;
  } else {
    outString = "render/render_" + outString;
  }
  #if MFluidSolver_USE_TBB
  tbb::parallel_for(tbb::blocked_range2d<unsigned int>(0, _width, 0, _height),
    [&](const tbb::blocked_range2d<unsigned int> &r) {
      for(unsigned int x = r.rows().begin(); x != r.rows().end(); ++x) {
        for(unsigned int y = r.cols().begin(); y != r.cols().end(); ++y) {
  #else
      for (unsigned int x = 0; x < _width; ++x) {
        for (unsigned int y = 0; y < _height / 2; ++y) {
  #endif
          for (unsigned int i = 0; i < 3; ++i) {
            screenshotArrayFlipped[3 * screenshotIndex(x, y) + i] = screenshotArray[3 * screenshotIndex(x, _height - 1 - y) + i];
            screenshotArrayFlipped[3 * screenshotIndex(x, _height - 1 - y) + i] = screenshotArray[3 * screenshotIndex(x, y) + i];
          }
  #if MFluidSolver_USE_TBB
        }
      }
    }
  );
  #else
        }
      }
  #endif

  // Write
  int result = stbi_write_tga(outString.c_str(), _width, _height, 3, screenshotArrayFlipped.data());
  if (manual) {
    if (result != 0) { // Success
      #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
      std::cout << "INFO: Wrote rendered image to: " << outString << std::endl;
      #endif
    } else { // Failure
      #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_ERROR
      std::cout << "ERROR: Failed to write rendered image to: " << outString << std::endl;
      #endif
    }
  }
}

void Viewer::configureScreenshot(bool render, unsigned int skip) {
  autoRender = render;
  renderSkip = skip;

  boost::filesystem::path renderDir("render");
  if (!boost::filesystem::create_directory(renderDir)) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_ERROR
    std::cout << "ERROR: Failed to create render folder. Disabling autorender" << std::endl;
    #endif
  }

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  if (autoRender) {
    if (renderSkip > 1) {
      std::cout << "INFO: Rendering 1 frame for every " << renderSkip << " to file!" << std::endl;
    } else {
      std::cout << "INFO: Rendering every frame to file!" << std::endl;
    }
  }
  #endif
}

void Viewer::stop() {
  shouldStop = true;
}

void Viewer::togglePause() {
  paused = !paused;

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  if (paused) std::cout << "INFO: Paused!" << std::endl;
  if (!paused) std::cout << "INFO: Unpaused!" << std::endl;
  #endif
}
