//
//  fluidSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_HPP_

#include <vector>
#include "../geom/geom.hpp"
#include "particle.hpp"

class FluidSolver {
public:
  FluidSolver();
  ~FluidSolver();

  virtual void update(double deltaT);

  virtual void addParticle(Particle *p);
  const std::vector<Particle *> *particles() const;
  unsigned int numParticles() const;
  unsigned int maxParticles;

  void setGravity(float g);
  float gravity() const;

  virtual void setParticleSeparation(float ps);
  float particleSeparation() const;

  Geometry *fluidSource;
  Geometry *fluidContainer;

private:
  std::vector<Particle *> *_particles;
  float _gravity;
  float _particleSeparation;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_HPP_ */
