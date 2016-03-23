//
//  sphSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_HPP_
#define MFLUIDSOLVER_SPHSOLVER_HPP_

#include <vector>
#include "../fluidSolver.hpp"
#include "kernelFunctions.hpp"
#include "neighborSearch.hpp"
#include "sphParticle.hpp"

#define SPHConfig_Default_kStiffness 1
#define SPHConfig_Default_muViscosity 1
#define SPHConfig_Default_rRadius 1
#define SPHConfig_Default_mMass 1000
#define SPHConfig_Default_dRestDensity 1000
#define SPHConfig_Default_dtTimestep 1

class SPHSolver : public FluidSolver {
public:
  SPHSolver();
  ~SPHSolver();

  void init(const double &kernelRadius,
    const glm::vec3 &gridMin, const glm::vec3 &gridMax, NeighborSearchType nsType);
  void setDefaultConfig();
  void loadConfig(const std::string &file);

  virtual void update(double deltaT);
  virtual void addParticleAt(const glm::vec3 &position);
  virtual std::vector<SPHParticle *> &particles();
  virtual unsigned int numParticles() const;
  virtual void setParticleSeparation(float ps);

  void demoCode(SPHParticle *target);
  void initialDemo();
  void randomDemo();

private:
  NeighborSearchType nSearchType;
  KernelFunctions kernelFunctions;
  NeighborSearch *nSearch;
  std::vector<SPHParticle *> _particles;

  float kStiffness;
  float muViscosity;
  float rRadius;
  float mMass;
  float dRestDensity;
  float dtTimestep;
};

#endif /* MFLUIDSOLVER_SPHSOLVER_HPP_ */