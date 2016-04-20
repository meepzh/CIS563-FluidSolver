//
//  iiSphSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_IISPHSOLVER_HPP_
#define MFLUIDSOLVER_SPHSOLVER_IISPHSOLVER_HPP_

#include "sphSolver.hpp"

#if MFluidSolver_PARTICLE_STATS
#include <mutex>
#endif

class IISPHSolver : public SPHSolver {
public:
  virtual void update(double deltaT);

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
  unsigned int numFlyaways;
  #endif

protected:
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
};

#endif /* MFLUIDSOLVER_SPHSOLVER_IISPHSOLVER_HPP_ */