//
//  fluidSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_HPP_

#include <vector>
#include "particle.hpp"

class FluidSolver {
public:
  FluidSolver();
  ~FluidSolver();

  void update(double deltaT);

  std::vector<Particle *> particles;
  unsigned int maxParticles;

  void setGravity(float g);
  float gravity() const;

  void setParticleSeparation(float ps);
  float particleSeparation() const;

private:
  float _gravity;
  float _particleSeparation;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_HPP_ */
