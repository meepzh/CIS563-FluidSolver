//
//  scene.cpp
//  MFluidSolver

#include "scene.hpp"

#include <fstream>
#include <iostream>
#include <json/json.h>

#include "geom/cube.hpp"
#include "utils.hpp"

Scene::Scene()
 : spawnMethod(ParticleSpawnMethod::Jittered) {
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

  // Load container info
  glm::vec3 containerDim;
  containerDim.x = root["containerDim"].get("scaleX", MFluidSolver_DEFAULT_SCENE_CONTAINER_SCALEX).asFloat();
  containerDim.y = root["containerDim"].get("scaleY", MFluidSolver_DEFAULT_SCENE_CONTAINER_SCALEY).asFloat();
  containerDim.z = root["containerDim"].get("scaleZ", MFluidSolver_DEFAULT_SCENE_CONTAINER_SCALEZ).asFloat();

  // Load source info
  glm::vec3 particleDim;
  particleDim.x = root["particleDim"].get("scaleX", MFluidSolver_DEFAULT_SCENE_SOURCE_SCALEX).asFloat();
  particleDim.y = root["particleDim"].get("scaleY", MFluidSolver_DEFAULT_SCENE_SOURCE_SCALEY).asFloat();
  particleDim.z = root["particleDim"].get("scaleZ", MFluidSolver_DEFAULT_SCENE_SOURCE_SCALEZ).asFloat();

  glm::vec3 particleConPos;
  particleConPos.x = root["particleDim"].get("posX", MFluidSolver_DEFAULT_SCENE_SOURCE_POSX).asFloat();
  particleConPos.y = root["particleDim"].get("posY", MFluidSolver_DEFAULT_SCENE_SOURCE_POSY).asFloat();
  particleConPos.z = root["particleDim"].get("posZ", MFluidSolver_DEFAULT_SCENE_SOURCE_POSZ).asFloat();

  // Get spawn method info
  std::string spawningMethodString = root.get("spawnMethod", MFluidSolver_DEFAULT_SPAWNMETHODSTRING).asString();
  MUtils::toLowerInplace(spawningMethodString);
  if (spawningMethodString == "jittered") {
    spawnMethod = ParticleSpawnMethod::Jittered;
  } else if (spawningMethodString == "poissondisk") {
    spawnMethod = ParticleSpawnMethod::PoissonDisk;
  } else if (spawningMethodString == "uniform") {
    spawnMethod = ParticleSpawnMethod::Uniform;
  }

  // Load camera info
  glm::vec3 cameraEye;
  cameraEye.x = root["camera"].get("eyeX", MFluidSolver_DEFAULT_SCENE_CAMERA_EYEX).asFloat();
  cameraEye.y = root["camera"].get("eyeY", MFluidSolver_DEFAULT_SCENE_CAMERA_EYEY).asFloat();
  cameraEye.z = root["camera"].get("eyeZ", MFluidSolver_DEFAULT_SCENE_CAMERA_EYEZ).asFloat();

  glm::vec3 cameraRef;
  cameraRef.x = root["camera"].get("refX", MFluidSolver_DEFAULT_SCENE_CAMERA_REFX).asFloat();
  cameraRef.y = root["camera"].get("refY", MFluidSolver_DEFAULT_SCENE_CAMERA_REFY).asFloat();
  cameraRef.z = root["camera"].get("refZ", MFluidSolver_DEFAULT_SCENE_CAMERA_REFZ).asFloat();

  // Set attributes
  camera.setEyeRef(cameraEye, cameraRef);

  float particleSeparation = root.get("particleSeparation", MFluidSolver_DEFAULT_PARTICLE_SEPARATION).asFloat();
  solver.setParticleSeparation(particleSeparation);

  int maxParticles = root.get("maxParticles", MFluidSolver_DEFAULT_MAX_PARTICLES).asInt();
  solver.setMaxParticles(maxParticles);

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
  solver.fluidSource->transform.setTransform(particleConPos, glm::vec3(0), particleDim);

  seedScene();

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: Particle count: " << solver.numParticles() << " / " << solver.maxParticles() << std::endl;
  #endif

  solver.sceneLoaded();
}

void Scene::seedScene() {
  solver.fluidSource->spawnParticlesInVolume(&solver, spawnMethod);
}