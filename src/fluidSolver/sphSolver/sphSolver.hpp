//
//  sphSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_HPP_
#define MFLUIDSOLVER_SPHSOLVER_HPP_

#include "../fluidSolver.hpp"

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

};

#endif /* MFLUIDSOLVER_SPHSOLVER_HPP_ */