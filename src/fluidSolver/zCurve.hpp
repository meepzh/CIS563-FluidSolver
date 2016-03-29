//
//  zCurve.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_ZCURVE_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_ZCURVE_HPP_

#include "MFluidSolverConfig.hpp"

#include <exception>
#include <glm/glm.hpp>

struct ZCurveNegativeBoundsException : std::exception {
  const char *what() const noexcept {return "ZCurve cannot be constructed with negative cell bounds.\n";};
};

struct ZCurveTooLargeException : std::exception {
  const char *what() const noexcept {return "ZCurve cannot use such high indices";};
};

class ZCurve {
public:
  ZCurve(const glm::ivec3 &cellBounds);

private:
  unsigned long getIndex(int i, int j, int k);
  unsigned long splitBits(unsigned long i);
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_ZCURVE_HPP_ */