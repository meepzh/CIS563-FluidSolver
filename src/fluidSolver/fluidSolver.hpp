//
//  fluidSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_HPP_

#include <vector>
#include "particle.hpp"

class FluidSolver {
public:
  ~FluidSolver();

  void update();

  std::vector<Particle *> particles;

  void setGravity(float g);
  float gravity();

private:
  float _gravity;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_HPP_ */
