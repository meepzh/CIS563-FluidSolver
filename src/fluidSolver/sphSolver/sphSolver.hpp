//
//  sphSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_HPP_
#define MFLUIDSOLVER_SPHSOLVER_HPP_

#include "MFluidSolverConfig.hpp"

#include <vector>

#include "../fluidSolver.hpp"
#include "kernelFunctions.hpp"
#include "neighborSearch.hpp"
#include "sphParticle.hpp"

class SPHSolver : public FluidSolver {
public:
  SPHSolver();
  ~SPHSolver();

  void init(const glm::vec3 &gridMin, const glm::vec3 &gridMax);
  void setDefaultConfig();
  void loadConfig(const std::string &file);

  virtual void update(double deltaT);
  virtual void addParticleAt(const glm::vec3 &position);
  virtual std::vector<SPHParticle> &particles();
  virtual unsigned int numParticles() const;
  virtual void setParticleSeparation(float ps);
  virtual void setMaxParticles(int mp);

  void prepNeighborSearchAfterSceneLoad();
  void prepNeighborSearch();
  void visualizeParticle0Neighbors();
  void visualizeRandomParticlesNeighbors();

  #if MFluidSolver_USE_OPENVDB
  void exportVDB();
  #endif

private:
  void visualizeParticleNeighbors(SPHParticle *target);

  NeighborSearchType nSearchType;
  KernelFunctions kernelFunctions;
  NeighborSearch *nSearch;
  std::vector<SPHParticle> _particles;

  float kStiffness;
  float muViscosity;
  float kernelRadius;
  float mMass;
  float dRestDensity;
  float dtTimestep;

  bool inited;
  bool checkInited();
};

#endif /* MFLUIDSOLVER_SPHSOLVER_HPP_ */