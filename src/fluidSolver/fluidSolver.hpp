//
//  fluidSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_HPP_

#include "MFluidSolverConfig.hpp"

#include <tbb/tick_count.h>

#include "geom/geom.hpp"
#include "particle.hpp"

enum FluidVisualizationType {Index, Neighbors, None, Pressure, Velocity, VelocityDir};

class FluidSolver {
public:
  FluidSolver();
  ~FluidSolver();

  // Solver
  virtual void update(double deltaT);
  void updateStep();

  // Particles
  virtual void addParticleAt(const glm::vec3 &position) = 0;
  virtual unsigned int numParticles() const = 0;

  // Getters
  float gravity() const;
  int maxParticles() const;
  float particleSeparation() const;

  // Setters
  void setFixedTimestep(float ft);
  void setGravity(float g);
  virtual void setMaxParticles(int mp);
  virtual void setParticleSeparation(float ps);

  // Simulation End
  inline bool checkIfEnded();
  inline bool hasEndedSimulation();
  void endSimulation();

  // Misc
  virtual void printPerformanceStats();
  unsigned int updateNumber();

  // Containers
  Geometry *fluidSource;
  Geometry *fluidContainer;

protected:
  // Helpers
  inline void logTimestep();

  // Parameters
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

#include "fluidSolver/fluidSolver.inline.hpp"

#endif /* MFLUIDSOLVER_FLUIDSOLVER_HPP_ */
