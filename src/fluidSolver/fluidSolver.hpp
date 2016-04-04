//
//  fluidSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_HPP_

#include "MFluidSolverConfig.hpp"

#include "../geom/geom.hpp"
#include "particle.hpp"

enum FluidVisualizationType {Neighbors, None, Velocity};

class FluidSolver {
public:
  FluidSolver();
  ~FluidSolver();

  virtual void update(double deltaT);
  void updateStep();

  virtual void addParticleAt(const glm::vec3 &position) = 0;
  virtual unsigned int numParticles() const = 0;
  //virtual std::vector<Particle *> &particles() = 0;

  void setGravity(float g);
  float gravity() const;
  virtual void setMaxParticles(int mp);
  int maxParticles() const;
  virtual void setParticleSeparation(float ps);
  float particleSeparation() const;
  void setFixedTimestep(float ft);

  virtual void printPerformanceStats();

  Geometry *fluidSource;
  Geometry *fluidContainer;

protected:
  float _gravity;
  float _particleSeparation;
  int _maxParticles;
  FluidVisualizationType visualizationType;
  float _fixedTimestep;

  double computeTime;
  unsigned int numUpdates;
  unsigned int maxUpdates;
  bool limitNumUpdates;
  bool endedSimulation;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_HPP_ */
