//
//  fluidSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_HPP_

#include "MFluidSolverConfig.hpp"

#include <tbb/tick_count.h>

#include "../geom/geom.hpp"
#include "particle.hpp"

enum FluidVisualizationType {Neighbors, None, Pressure, Velocity, VelocityDir};

class FluidSolver {
public:
  FluidSolver();
  ~FluidSolver();

  virtual void update(double deltaT);
  void updateStep();

  virtual void addParticleAt(const glm::vec3 &position) = 0;
  virtual unsigned int numParticles() const = 0;

  void setFixedTimestep(float ft);
  void setGravity(float g);
  virtual void setMaxParticles(int mp);
  virtual void setParticleSeparation(float ps);

  float gravity() const;
  int maxParticles() const;
  float particleSeparation() const;

  bool hasEndedSimulation();
  void endSimulation();

  virtual void printPerformanceStats();

  Geometry *fluidSource;
  Geometry *fluidContainer;

protected:
  float _gravity;
  float _particleSeparation;
  int _maxParticles;
  float _fixedTimestep;

  // Visualization
  FluidVisualizationType visualizationType;
  float visualizationMaxPressure;
  float visualizationMaxVelocity;
  glm::vec3 visualizationVelocityColor;

  // Performance measuring
  double computeTime;
  tbb::tick_count startTime, endTime;
  unsigned int numUpdates;
  unsigned int maxUpdates;
  bool limitNumUpdates;
  bool firstRun, endedSimulation;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_HPP_ */
