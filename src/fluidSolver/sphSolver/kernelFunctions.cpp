//
//  kernelFunctions.cpp
//  MFluidSolver

#include "kernelFunctions.hpp"
#include <cmath>

#define GLM_FORCE_RADIANS
#include <glm/gtx/norm.hpp>

#define POLY6_CONST 1.566681471060844711474949545698188251276712137757618167359
#define SPIKY_CONST 4.774648292756860073066512901175430861033789372213693462430

double KernelFunctions::_h = 0.1f;
double KernelFunctions::_h2 = 0.01f;
double KernelFunctions::_h3 = 0.001f;
double KernelFunctions::_h6 = 0.000001f;
double KernelFunctions::_h9 = 0.000000001f;

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