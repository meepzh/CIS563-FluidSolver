//
//  scene.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SCENE_HPP_
#define MFLUIDSOLVER_SCENE_HPP_

#include <string>
#include <vector>
#include "../geom/geom.hpp"

class Scene {
  std::vector<Geometry *> geometry;
  void loadScene(std::string file);
};

#endif /* MFLUIDSOLVER_SCENE_HPP_ */
