//  Copyright 2016 Robert Zhou
//
//  kernelFunctions.inline.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_KERNELFUNCTIONS_INLINE_HPP_
#define MFLUIDSOLVER_SPHSOLVER_KERNELFUNCTIONS_INLINE_HPP_

#define GLM_FORCE_RADIANS
#include <glm/gtx/norm.hpp>

// Super long constants
// constexpr double POLY6_CONST = 1.5666814710608447114749495456981882512;
// constexpr double SPIKY_CONST = 4.7746482927568600730665129011754308610;
// constexpr double EIGHT_DIV_PI = 2.5464790894703253723021402139602297925;
// constexpr double VISCOUS_CONST = 2.3873241463784300365332564505877154305;
// constexpr double FORTYFIVE_DIV_PI = 14.323944878270580219199538703526292583;
// constexpr double FORTYEIGHT_DIV_PI = 15.278874536821952233812841283761378755;

// Shorter constants
constexpr double POLY6_CONST = 1.56668147106084471;
constexpr double SPIKY_CONST = 4.77464829275686007;
constexpr double EIGHT_DIV_PI = 2.54647908947032537;
constexpr double VISCOUS_CONST = 2.38732414637843004;
constexpr double FORTYFIVE_DIV_PI = 14.323944878270580;
constexpr double FORTYEIGHT_DIV_PI = 15.278874536821952;

// Bicubic spline functions according to Ihmsen thesis
inline double KernelFunctions::computeBicubicSpline(const glm::vec3 &r) {
  double q = glm::length(r) / _h;
  if (q <= 0.5) {
    return EIGHT_DIV_PI / _h3 * 6 * (q * q * q - q * q) + 1;
  } else {
    // Assumes q <= 1
    double invQ = 1 - q;
    return EIGHT_DIV_PI / _h3 * 2 * invQ * invQ * invQ;
  }
}

inline double KernelFunctions::computeBicubicSplineFirstDerivativeDivHHalf(
    const double q) {
  return FORTYEIGHT_DIV_PI / _h4 * (3 * q * q - 2 * q);
}

inline double KernelFunctions::computeBicubicSplineFirstDerivativeDivHWhole(
    const double invQ) {
  return FORTYEIGHT_DIV_PI / _h4 * (-1 * invQ * invQ);
}

inline glm::vec3 KernelFunctions::computeBicubicSplineGradient(
    const glm::vec3 &r) {
  double rLen = glm::length(r);
  double q = rLen / _h;
  if (q <= 0.5) {
    return static_cast<float>(
      computeBicubicSplineFirstDerivativeDivHHalf(q) / rLen) * r;
  } else {
    return static_cast<float>(
      computeBicubicSplineFirstDerivativeDivHWhole(1 - q) / rLen) * r;
  }
}

inline double KernelFunctions::computeBicubicSplineLaplacian(
    const glm::vec3 &r) {
  double rLen = glm::length(r);
  double q = rLen / _h;
  if (q <= 0.5) {
    return FORTYEIGHT_DIV_PI * (6 * q - 2) / _h5 +
      computeBicubicSplineFirstDerivativeDivHHalf(q) * 2 / rLen;
  } else {
    return FORTYEIGHT_DIV_PI * (2 - 2 * q) / _h5 +
      computeBicubicSplineFirstDerivativeDivHWhole(1 - q) * 2 / rLen;
  }
}

// Standard functions according to Muller 2003
inline double KernelFunctions::computePoly6(const glm::vec3 &r) {
  double _r2 = glm::length2(r);
  if (_r2 > _h2) return 0;
  double h2MinusR2 = _h2 - _r2;
  return POLY6_CONST * h2MinusR2 * h2MinusR2 * h2MinusR2 / _h9;
}

inline double KernelFunctions::computeSpiky(const glm::vec3 &r) {
  double _r = glm::length(r);
  double hMinusR = _h - _r;
  return SPIKY_CONST * hMinusR * hMinusR * hMinusR / _h6;
}

inline glm::vec3 KernelFunctions::computeSpikyGradient(const glm::vec3 &r) {
  double _r = glm::length(r);
  if (_r == 0) return glm::vec3(0);
  double hMinusR = _h - _r;
  return static_cast<float>(
    -1 * FORTYFIVE_DIV_PI * hMinusR * hMinusR / _r /_h6) * r;
}

inline double KernelFunctions::computeViscous(const glm::vec3 &r) {
  double _r = glm::length(r);
  return VISCOUS_CONST *
    (-0.5 * _r * _r * _r / _h3 +
    _r * _r / _h2 +
    0.5 * _h / _r - 1) / _h3;
}

inline double KernelFunctions::computeViscousLaplacian(const glm::vec3 &r) {
  double _r = glm::length(r);
  return FORTYFIVE_DIV_PI * (_h - _r) / _h6;
}

#endif  // MFLUIDSOLVER_SPHSOLVER_KERNELFUNCTIONS_INLINE_HPP_
