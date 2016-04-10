//
//  kernelFunctions.inline.hpp
//  MFluidSolver

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
  return (float)(-1 * FORTYFIVE_DIV_PI * hMinusR * hMinusR / _r /_h6) * r;
}

inline double KernelFunctions::computeViscous(const glm::vec3 &r) {
  double _r = glm::length(r);
  return VISCOUS_CONST * (-0.5 * _r * _r * _r / _h3 + _r * _r / _h2 + 0.5 * _h / _r - 1) / _h3;
}

inline double KernelFunctions::computeViscousLaplacian(const glm::vec3 &r) {
  double _r = glm::length(r);
  return FORTYFIVE_DIV_PI * (_h - _r) / _h6;
}