//
//  scene.cpp
//  MFluidSolver

#include "scene.hpp"

#include <fstream>
#include <json/json.h>

Scene::Scene()
 : fluidSource(nullptr), fluidContainer(nullptr) {
}

Scene::~Scene() {
  for (std::vector<Geometry *>::iterator it = geometry.begin(); it != geometry.end(); ++it) {
    delete (*it);
  }
}

void Scene::loadJSON(std::string file) {
  // Read JSON file
  Json::Reader reader;
  Json::Value root;
  std::ifstream sceneStream(file, std::ifstream::binary);

  bool success = reader.parse(sceneStream, root, false);
  if (!success) {
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
  particleDim *= 2;

  float particleSeparation = root.get("particleSeparation", 0.1f).asFloat();

  // Create geometry
  fluidContainer = new Cube(glm::vec3(0));
  fluidSource = new Cube(glm::vec3(0));
  geometry.push_back(fluidContainer);
  geometry.push_back(fluidSource);

  // Change geometry scale
  fluidContainer->transform.setScale(containerDim);
  fluidSource->transform.setScale(particleDim);
}