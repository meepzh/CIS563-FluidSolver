//
//  iiSphSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_IISPHSOLVER_HPP_
#define MFLUIDSOLVER_SPHSOLVER_IISPHSOLVER_HPP_

#include "sphSolver.hpp"

class IISPHSolver : public SPHSolver {
public:
  virtual void update(double deltaT);

protected:
  #if MFluidSolver_PARTICLE_STATS
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
  unsigned int totalFlyaways;
  #endif
};

#endif /* MFLUIDSOLVER_SPHSOLVER_IISPHSOLVER_HPP_ */