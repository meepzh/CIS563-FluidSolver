//
//  fluidSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_HPP_

#include <vector>
#include "particle.hpp"

class Geometry;

class FluidSolver {
public:
  FluidSolver();
  ~FluidSolver();

  void update(double deltaT);

  void addParticle(Particle *p);
  const std::vector<Particle *> *particles() const;
  unsigned int numParticles() const;
  unsigned int maxParticles;

  void setGravity(float g);
  float gravity() const;

  void setParticleSeparation(float ps);
  float particleSeparation() const;

  Geometry *fluidSource;
  Geometry *fluidContainer;

private:
  std::vector<Particle *> *_particles;
  float _gravity;
  float _particleSeparation;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_HPP_ */
