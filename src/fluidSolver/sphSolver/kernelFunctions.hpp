//
//  kernelFunctions.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_KERNELFUNCTIONS_HPP_
#define MFLUIDSOLVER_SPHSOLVER_KERNELFUNCTIONS_HPP_

#include <glm/glm.hpp>

class KernelFunctions {
public:
  KernelFunctions();

  void setKernelRadius(const double &h);

  inline double computePoly6(const glm::vec3 &r);
  inline double computeSpiky(const glm::vec3 &r);
  inline glm::vec3 computeSpikyGradient(const glm::vec3 &r);
  inline double computeViscous(const glm::vec3 &r);
  inline double computeViscousLaplacian(const glm::vec3 &r);

private:
  double _h;
  double _h2;
  double _h3;
  double _h6;
  double _h9;
};

#include "kernelFunctions.inline.hpp"

#endif /* MFLUIDSOLVER_SPHSOLVER_KERNELFUNCTIONS_HPP_ */