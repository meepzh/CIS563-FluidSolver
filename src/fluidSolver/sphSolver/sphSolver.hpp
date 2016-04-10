//
//  sphSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_HPP_
#define MFLUIDSOLVER_SPHSOLVER_HPP_

#include "MFluidSolverConfig.hpp"

#include <vector>

#include "fluidSolver/fluidSolver.hpp"
#include "kernelFunctions.hpp"
#include "neighborSearch.hpp"
#include "sphParticle.hpp"

#if MFluidSolver_USE_TBB
  #include <tbb/parallel_for.h>
  #define iter_all_sphparticles_start tbb::parallel_for(tbb::blocked_range<size_t>(0, _particles.size()), \
    [&](const tbb::blocked_range<size_t> &r) { \
      for (unsigned int i = r.begin(); i != r.end(); ++i) { \
        SPHParticle &p = _particles.at(i);
  #define iter_all_sphparticles_end }});
#else
  #define iter_all_sphparticles_start for (unsigned int i = 0; i < _particles.size(); ++i) { \
    SPHParticle &p = _particles.at(i);
  #define iter_all_sphparticles_end }
#endif

class SPHSolver : public FluidSolver {
public:
  // Constructor / Destructor
  SPHSolver();
  ~SPHSolver();

  // Configuration
  void init(const glm::vec3 &gridMin, const glm::vec3 &gridMax);
  void setDefaultConfig();
  void loadConfig(const std::string &file);

  // Solver!
  virtual void update(double deltaT);

  // Particles
  virtual void addParticleAt(const glm::vec3 &position);
  virtual unsigned int numParticles() const;
  virtual std::vector<SPHParticle> &particles();
  virtual void setMaxParticles(int mp);
  virtual void setParticleSeparation(float ps);

  // Neighbor Visualization
  virtual void visualizeParticleNeighbors(SPHParticle *target);
  virtual void visualizeParticle0Neighbors();
  virtual void visualizeRandomParticlesNeighbors();

  // Misc
  virtual void prepNeighborSearchAfterSceneLoad();
  virtual void printPerformanceStats();

  #if MFluidSolver_USE_OPENVDB
  virtual void exportVDB();
  #endif

protected:
  // Initialization
  virtual bool checkInited();
  bool inited;

  // Neighbor Search
  virtual inline void prepNeighborSearch();
  virtual inline void runNeighborSearch();

  // Helpers
  inline void calculateDensity(SPHParticle &p);
  virtual inline void calculateNonPressureForce(SPHParticle &p);
  inline void enforceBounds(SPHParticle &p);
  virtual inline void visualizeParticle(SPHParticle &p);

  // SPH Required Objects
  NeighborSearchType nSearchType;
  KernelFunctions kernelFunctions;
  NeighborSearch *nSearch;
  std::vector<SPHParticle> _particles;

  // Configuration
  float kStiffness;
  float muViscosity;
  float kernelRadius;
  float mMass;
  float dRestDensity;
  float dtTimestep;
};

#include "sphSolver.inline.hpp"

#endif /* MFLUIDSOLVER_SPHSOLVER_HPP_ */