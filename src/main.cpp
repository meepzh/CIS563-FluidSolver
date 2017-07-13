//  Copyright 2016 Robert Zhou
//
//  main.cpp
//  MFluidSolver

// OpenGL Includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Misc C Includes
#include <json/json.h>
#include <ctime>
#include <fstream>

#include "MFluidSolverConfig.hpp"

// Optional C Includes
#if MFluidSolver_USE_OPENVDB
#include <openvdb/openvdb.h>
#endif
#if MFluidSolver_USE_TBB
#include <tbb/task_scheduler_init.h>
#endif

// Misc C++ Includes
#include <glm/glm.hpp>

// Project Includes
#include "viewer/input.hpp"
#include "viewer/particleShaderProgram.hpp"

#define MFluidSolver_MAIN_CATCH_EXCEPTIONS 0

int main() {
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  // Print version info
  std::cout << "INFO: Version " <<
    MFluidSolver_VERSION_MAJOR << "." <<
    MFluidSolver_VERSION_MINOR << std::endl;
  #endif

  // Initialize GLFW
  if (!glfwInit()) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_ERROR
    std::cerr << "FATAL: Failed to initiaize GLFW" << std::endl;
    #endif

    getchar(); // Wait for key before quit
    return -1;
  }

  // Seed randomness
  std::time_t seed = std::time(NULL);
  std::srand(seed);
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: std::rand seed: " << seed << std::endl;
  #endif

  // Initialize non-OpenGL
  #if MFluidSolver_USE_OPENVDB
  openvdb::initialize();
  #endif
  #if MFluidSolver_USE_TBB
  tbb::task_scheduler_init init(tbb::task_scheduler_init::automatic);
  #endif

  // Set JSON file location
  std::string configJSON = MFluidSolver_DEFAULT_CONFIG_FILE;
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: Loading config file: " << configJSON << std::endl;
  #endif

  // Open JSON file
  Json::Reader reader;
  Json::Value root;
  std::ifstream configStream(configJSON, std::ifstream::binary);
  bool success = reader.parse(configStream, root, false);
  if (!success) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_ERROR
    std::cerr << "ERROR: Failed to parse config file " <<
      configJSON.c_str() << std::endl;
    #endif
  }

  // Parse JSON
  std::string sceneJSON = root.get(
    "sceneJSON", MFluidSolver_DEFAULT_SCENE_FILE).asString();
  std::string wireVShader = root.get(
    "wireVShader", MFluidSolver_DEFAULT_WIRE_VERT_FILE).asString();
  std::string wireFShader = root.get(
    "wireFShader", MFluidSolver_DEFAULT_WIRE_FRAG_FILE).asString();
  std::string particleVShader = root.get(
    "particleVShader", MFluidSolver_DEFAULT_PARTICLE_VERT_FILE).asString();
  std::string particleFShader = root.get(
    "particleFShader", MFluidSolver_DEFAULT_PARTICLE_FRAG_FILE).asString();
  std::string particleTexture = root.get(
    "particleTexture", MFluidSolver_DEFAULT_PARTICLE_TEX_FILE).asString();
  bool autoRender = root.get(
    "autoRender", MFluidSolver_DEFAULT_AUTORENDER).asBool();
  unsigned int renderSkip = root.get(
    "renderSkip", MFluidSolver_DEFAULT_RENDERSKIP).asInt();

  // Initialize project objects
  Viewer viewer;
  Input::viewer = &viewer;
  try {
    viewer.init();
  } catch (std::exception &e) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_FATAL
    std::cerr << "FATAL: " << e.what() << std::endl;
    #endif
    return -1;
  }

  // Print OpenGL info
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: OpenGL Version " << glGetString(GL_VERSION) << std::endl;
  #endif

  // Set a few settings/modes in OpenGL rendering
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

  // Set the size with which points should be rendered
  glPointSize(MFluidSolver_DEFAULT_POINT_SIZE);

  // Set background color
  glClearColor(MFluidSolver_DEFAULT_BG_COLOR_R,
               MFluidSolver_DEFAULT_BG_COLOR_G,
               MFluidSolver_DEFAULT_BG_COLOR_B,
               0.0f);

  // Bind vertex array object
  GLuint vaoID;
  glGenVertexArrays(1, &vaoID);
  glBindVertexArray(vaoID);

  // After vertex array object init
  viewer.wireShader = new ShaderProgram(wireVShader, wireFShader);
  viewer.particleShader =
    new ParticleShaderProgram(&(viewer.scene.solver),
      particleVShader, particleFShader, particleTexture);
  viewer.scene.solver.loadConfig(configJSON);
  viewer.configureScreenshot(autoRender, renderSkip);

  // Run! Catch and print exceptions as necessary
  int returnCode = 0;
  #if MFluidSolver_MAIN_CATCH_EXCEPTIONS
  try {
  #endif
    viewer.scene.loadJSON(sceneJSON);
    viewer.particleShader->init();
    viewer.run();
  #if MFluidSolver_MAIN_CATCH_EXCEPTIONS
  } catch (std::exception &e) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_FATAL
    std::cerr << "FATAL: " << e.what() << std::endl;
    #endif
    returnCode = -1;
  }
  #endif

  // Performance
  #if MFluidSolver_RECORD_PERFORMANCE
  viewer.scene.solver.endSimulation();
  viewer.scene.solver.printPerformanceStats();
  #endif

  // Cleanup
  glDeleteVertexArrays(1, &vaoID);

  return returnCode;
}
