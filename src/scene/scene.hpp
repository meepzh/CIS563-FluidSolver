//
//  scene.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SCENE_HPP_
#define MFLUIDSOLVER_SCENE_HPP_

#include <string>
#include <vector>
#include "../fluidSolver/fluidSolver.hpp"
#include "../geom/cube.hpp"

class Scene {
public:
  Scene();
  ~Scene();
  void loadJSON(const std::string &file);

  FluidSolver solver;
  std::vector<Geometry *> objects;
  Geometry *fluidSource;
  Geometry *fluidContainer;
};

#endif /* MFLUIDSOLVER_SCENE_HPP_ */
