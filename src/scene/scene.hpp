//  Copyright 2016 Robert Zhou
//
//  scene.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SCENE_HPP_
#define MFLUIDSOLVER_SCENE_HPP_

#include <exception>
#include <string>
#include <vector>

#include "MFluidSolverConfig.hpp"

#include "fluidSolver/sphSolver/iiSphSolver.hpp"
#include "camera.hpp"

struct InvalidSceneException : std::exception {
  const char *what() const noexcept {return "Invalid scene.\n";};
};

class Scene {
 public:
  Scene();
  ~Scene();
  void loadJSON(const std::string &file);
  void seedScene();

  IISPHSolver solver;
  std::vector<Geometry *> objects;
  Camera camera;

 private:
  ParticleSpawnMethod spawnMethod;
};

#endif  // MFLUIDSOLVER_SCENE_HPP_
