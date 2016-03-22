//
//  kernelFunctions.cpp
//  MFluidSolver

#include "kernelFunctions.hpp"
#include <cmath>

#define GLM_FORCE_RADIANS
#include <glm/gtx/norm.hpp>

#define POLY6_CONST 1.566681471060844711474949545698188251276712137757618167359
#define SPIKY_CONST 4.774648292756860073066512901175430861033789372213693462430

KernelFunctions::KernelFunctions() {
  setKernelRadius(0.1);
}

void KernelFunctions::setKernelRadius(const double &h) {
  _h = h;
  _h2 = h * h;
  _h3 = _h2 * h;
  _h6 = _h3 * _h3;
  _h9 = _h6 * _h3;
}

double KernelFunctions::computePoly6(const glm::vec3 &r) {
  return POLY6_CONST * std::pow(_h2 - glm::length2(r), 3) / _h9;
}

double KernelFunctions::computeSpiky(const glm::vec3 &r) {
  return SPIKY_CONST * std::pow(_h - glm::length(r), 3) / _h6;
}