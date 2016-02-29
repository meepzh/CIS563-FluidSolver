//
//  scene.cpp
//  MFluidSolver

#include "scene.hpp"

#include <cstdio>
#include <fstream>
#include <json/json.h>

Scene::Scene()
 : fluidSource(nullptr), fluidContainer(nullptr) {
}

Scene::~Scene() {
  for (Geometry *g : objects) {
    delete g;
  }
}

void Scene::loadJSON(std::string file) {
  // Read JSON file
  Json::Reader reader;
  Json::Value root;
  std::ifstream sceneStream(file, std::ifstream::binary);

  bool success = reader.parse(sceneStream, root, false);
  if (!success) {
    std::printf("Failed to parse scene file %s.", file);
    return;
  }

  glm::vec3 containerDim;
  containerDim.x = root["containerDim"].get("scaleX", 1.f).asFloat();
  containerDim.y = root["containerDim"].get("scaleY", 1.f).asFloat();
  containerDim.z = root["containerDim"].get("scaleZ", 1.f).asFloat();

  glm::vec3 particleDim;
  particleDim.x = root["particleDim"].get("boundX", 0.5f).asFloat();
  particleDim.y = root["particleDim"].get("boundY", 0.5f).asFloat();
  particleDim.z = root["particleDim"].get("boundZ", 0.5f).asFloat();

  float particleSeparation = root.get("particleSeparation", 0.1f).asFloat();

  // Create geometry
  fluidContainer = new Cube(glm::vec3(0));
  fluidSource = new Cube(glm::vec3(0));
  objects.push_back(fluidContainer);
  objects.push_back(fluidSource);

  // Change geometry scale
  fluidContainer->transform.setScale(containerDim);
  fluidSource->transform.setScale(particleDim);
}