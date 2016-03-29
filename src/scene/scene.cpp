//
//  scene.cpp
//  MFluidSolver

#include "scene.hpp"

#include <fstream>
#include <iostream>
#include <json/json.h>
#include "../geom/cube.hpp"

Scene::Scene() {
}

Scene::~Scene() {
  for (Geometry *g : objects) {
    delete g;
  }
}

void Scene::loadJSON(const std::string &file) {
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: Loading scene file: " << file << std::endl;
  #endif

  // Read JSON file
  Json::Reader reader;
  Json::Value root;
  std::ifstream sceneStream(file, std::ifstream::binary);

  bool success = reader.parse(sceneStream, root, false);
  if (!success) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_FATAL
    std::cerr << "FATAL: Failed to parse scene file " << std::endl;
    #endif

    throw InvalidSceneException();
  }

  glm::vec3 containerDim;
  containerDim.x = root["containerDim"].get("scaleX", 1.f).asFloat();
  containerDim.y = root["containerDim"].get("scaleY", 1.f).asFloat();
  containerDim.z = root["containerDim"].get("scaleZ", 1.f).asFloat();

  glm::vec3 particleDim;
  particleDim.x = root["particleDim"].get("scaleX", 0.5f).asFloat();
  particleDim.y = root["particleDim"].get("scaleY", 0.5f).asFloat();
  particleDim.z = root["particleDim"].get("scaleZ", 0.5f).asFloat();

  float particleSeparation = root.get("particleSeparation", 0.1f).asFloat();
  solver.setParticleSeparation(particleSeparation);

  solver.init(containerDim * -0.5f, containerDim * 0.5f);

  // Create geometry
  solver.fluidContainer = new Cube(glm::vec3(0));
  solver.fluidContainer->name = "Fluid Container";
  solver.fluidSource = new Cube(glm::vec3(0));
  solver.fluidSource->name = "Fluid Source";
  objects.push_back(solver.fluidContainer);
  objects.push_back(solver.fluidSource);

  // Change geometry scale
  solver.fluidContainer->transform.setScale(containerDim);
  solver.fluidSource->transform.setScale(particleDim);

  seedScene();

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: Particle count: " << solver.numParticles() << " / " solver.maxParticles << std::endl;
  #endif

  solver.initialDemo();
}

void Scene::seedScene() {
  solver.fluidSource->spawnParticlesInVolume(&solver);
}