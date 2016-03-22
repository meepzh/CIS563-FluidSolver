//
//  sphSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_HPP_
#define MFLUIDSOLVER_SPHSOLVER_HPP_

#include "../fluidSolver.hpp"
#include "kernelFunctions.hpp"
#include "neighborSearch.hpp"

struct SPHConfig {
  float kStiffness;
  float muViscosity;
  float rRadius;
  float mMass;
  float dRestDensity;
  float dtTimestep;
};

class SPHSolver : public FluidSolver {
public:
  SPHSolver();
  ~SPHSolver();

  SPHConfig *init(const double &kernelRadius,
    const glm::vec3 &gridMin, const glm::vec3 &gridMax, NeighborSearchType nSearchType);
  void setDefaultConfig();

  virtual void update(double deltaT);
  virtual void addParticle(Particle *p);
  virtual void setParticleSeparation(float ps);

private:
  SPHConfig config;
  KernelFunctions kernelFunctions;
  NeighborSearch *nSearch;
};

#endif /* MFLUIDSOLVER_SPHSOLVER_HPP_ */