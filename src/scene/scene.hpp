//
//  scene.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SCENE_HPP_
#define MFLUIDSOLVER_SCENE_HPP_

#include <string>
#include <vector>
#include "../geom/cube.hpp"

class Scene {
public:
  Scene();
  ~Scene();
  void loadScene(std::string file);

  std::vector<Geometry *> geometry;
  Geometry *fluidSource;
  Geometry *fluidContainer;
};

#endif /* MFLUIDSOLVER_SCENE_HPP_ */
