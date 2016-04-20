//  Copyright 2016 Robert Zhou
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

  // Bicubic spline functions according to Ihmsen thesis
  inline double computeBicubicSpline(const glm::vec3 &r);
  inline double computeBicubicSplineFirstDerivativeDivHHalf(const double q);
  inline double computeBicubicSplineFirstDerivativeDivHWhole(const double invQ);
  inline glm::vec3 computeBicubicSplineGradient(const glm::vec3 &r);
  inline double computeBicubicSplineLaplacian(const glm::vec3 &r);

  // Standard functions according to Muller 2003
  inline double computePoly6(const glm::vec3 &r);
  inline double computeSpiky(const glm::vec3 &r);
  inline glm::vec3 computeSpikyGradient(const glm::vec3 &r);
  inline double computeViscous(const glm::vec3 &r);
  inline double computeViscousLaplacian(const glm::vec3 &r);

 private:
  double _h;
  double _h2;
  double _h3;
  double _h4;
  double _h5;
  double _h6;
  double _h9;
};

#include "kernelFunctions.inline.hpp"

#endif  // MFLUIDSOLVER_SPHSOLVER_KERNELFUNCTIONS_HPP_
