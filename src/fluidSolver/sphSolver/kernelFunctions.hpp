//
//  kernelFunctions.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_KERNELFUNCTIONS_HPP_
#define MFLUIDSOLVER_SPHSOLVER_KERNELFUNCTIONS_HPP_

#include <glm/glm.hpp>

class KernelFunctions {
public:
  static void setKernelRadius(const double &h);
  static double computePoly6(const glm::vec3 &r);
  static double computeSpiky(const glm::vec3 &r);

private:
  static double _h;
  static double _h2;
  static double _h3;
  static double _h6;
  static double _h9;
};

#endif /* MFLUIDSOLVER_SPHSOLVER_KERNELFUNCTIONS_HPP_ */