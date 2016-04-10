//
//  iiSphSolver.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_IISPHSOLVER_HPP_
#define MFLUIDSOLVER_SPHSOLVER_IISPHSOLVER_HPP_

#include "sphSolver.hpp"

class IISPHSolver : public SPHSolver {
public:
  virtual void update(double deltaT);
};

#endif /* MFLUIDSOLVER_SPHSOLVER_IISPHSOLVER_HPP_ */