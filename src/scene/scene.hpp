//
//  scene.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SCENE_HPP_
#define MFLUIDSOLVER_SCENE_HPP_

#include "MFluidSolverConfig.hpp"

#include <exception>
#include <string>
#include <vector>

#include "../fluidSolver/sphSolver/sphSolver.hpp"
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

  SPHSolver solver;
  std::vector<Geometry *> objects;
  Camera camera;
};

#endif /* MFLUIDSOLVER_SCENE_HPP_ */
