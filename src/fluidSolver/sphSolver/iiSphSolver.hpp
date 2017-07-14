//  Copyright 2016 Robert Zhou
//
//  iiSphSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_IISPHSOLVER_HPP_
#define MFLUIDSOLVER_SPHSOLVER_IISPHSOLVER_HPP_

#include <string>

#include "sphSolver.hpp"

#if MFluidSolver_PARTICLE_STATS
#include <mutex>
#endif

class IISPHSolver : public SPHSolver {
 public:
  IISPHSolver();

  virtual void update(double deltaT);
  virtual void loadConfig(const std::string &file);

  virtual void calculateParticleMass(float particleSeparation);

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
  bool diverging;
  #endif

 protected:
  float densityTolerance;
  unsigned int maxIterations;

  #if MFluidSolver_PARTICLE_STATS
  std::mutex statsCoutMutex;
  glm::vec3 averagePosition;
  float averageVelocityMagnitude;
  float averageDensity;
  float averagePressure;
  float averageNonPressureForceMagnitude;
  float averagePressureForceMagnitude;
  float averageAdvectionDiagonal;
  float averageAdvectionDisplacementMagnitude;
  float averageDensityIntermediate;
  float averageSumPressureDisplacementFromNeighborsMagnitude;
  float averageVelocityIntermediateMagnitude;
  float averageNumNeighbors;
  #endif

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
  float lastDensityDifference;
  #endif
};

#endif  // MFLUIDSOLVER_SPHSOLVER_IISPHSOLVER_HPP_
