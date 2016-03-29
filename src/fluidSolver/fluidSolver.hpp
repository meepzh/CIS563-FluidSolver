//
//  fluidSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_HPP_

#include "MFluidSolverConfig.hpp"

#include "../geom/geom.hpp"
#include "particle.hpp"

class FluidSolver {
public:
  FluidSolver();
  ~FluidSolver();

  virtual void update(double deltaT);
  void updateStep();

  virtual void addParticleAt(const glm::vec3 &position) = 0;
  virtual unsigned int numParticles() const = 0;
  //virtual std::vector<Particle *> &particles() = 0;
  unsigned int maxParticles;

  void setGravity(float g);
  float gravity() const;

  virtual void setParticleSeparation(float ps);
  float particleSeparation() const;

  Geometry *fluidSource;
  Geometry *fluidContainer;

protected:
  float _gravity;
  float _particleSeparation;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_HPP_ */
