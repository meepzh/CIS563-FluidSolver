//
//  scene.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SCENE_HPP_
#define MFLUIDSOLVER_SCENE_HPP_

#include <string>
#include <vector>
#include "../fluidSolver/sphSolver/sphSolver.hpp"

class Scene {
public:
  Scene();
  ~Scene();
  void loadJSON(const std::string &file);

  SPHSolver solver;
  std::vector<Geometry *> objects;
};

#endif /* MFLUIDSOLVER_SCENE_HPP_ */
