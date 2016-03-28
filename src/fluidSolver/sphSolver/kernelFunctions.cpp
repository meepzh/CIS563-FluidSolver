//
//  kernelFunctions.cpp
//  MFluidSolver

#include "kernelFunctions.hpp"
#include <cmath>

#define GLM_FORCE_RADIANS
#include <glm/gtx/norm.hpp>

//#define POLY6_CONST 1.566681471060844711474949545698188251276712137757618167359
//#define SPIKY_CONST 4.774648292756860073066512901175430861033789372213693462430
//#define VISCOUS_CONST 2.387324146378430036533256450587715430516894686106846731215
//#define FORTYFIVE_DIV_PI 14.32394487827058021919953870352629258310136811664108038729
#define POLY6_CONST 1.56668147106084471
#define SPIKY_CONST 4.77464829275686007
#define VISCOUS_CONST 2.38732414637843004
#define FORTYFIVE_DIV_PI 14.323944878270580

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
  double _r2 = glm::length2(r);
  if (_r2 > _h2) return 0;
  return POLY6_CONST * std::pow(_h2 - _r2, 3) / _h9;
}

double KernelFunctions::computeSpiky(const glm::vec3 &r) {
  double _r = glm::length(r);
  if (_r > _h) return 0;
  return SPIKY_CONST * std::pow(_h - _r, 3) / _h6;
}

glm::vec3 KernelFunctions::computeSpikyGradient(const glm::vec3 &r) {
  double _r = glm::length(r);
  //if (_r == 0) return glm::vec3((float) -1 * FORTYFIVE_DIV_PI / _h6);
  if (_r == 0 || _r > _h) return glm::vec3(0);
  double hMinusR = _h - _r;
  return (float)(-1 * FORTYFIVE_DIV_PI * hMinusR * hMinusR / _r /_h6) * r;
}

double KernelFunctions::computeViscous(const glm::vec3 &r) {
  double _r = glm::length(r);
  if (_r > _h) return 0;
  return VISCOUS_CONST * (-0.5 * _r * _r * _r / _h3 + _r * _r / _h2 + 0.5 * _h / _r - 1) / _h3;
}

double KernelFunctions::computeViscousLaplacian(const glm::vec3 &r) {
  double _r = glm::length(r);
  if (_r > _h) return 0;
  return FORTYFIVE_DIV_PI * (_h - _r) / _h6;
}