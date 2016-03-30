//
//  zCurve.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_ZCURVE_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_ZCURVE_HPP_

#include "MFluidSolverConfig.hpp"

#include <exception>
#include <glm/glm.hpp>

#if MFluidSolver_ZCURVE_CACHING
#include <vector>
#endif

#if MFluidSolver_ZCURVE_CACHING
struct simpleIvec3Comparator {
  bool operator()(const glm::ivec3 &a, const glm::ivec3 &b) const {
    if (a.x != b.x) return a.x < b.x;
    if (a.y != b.y) return a.y < b.y;
    return a.z < b.z;
  }
};
#endif

struct ZCurveNegativeBoundsException : std::exception {
  const char *what() const noexcept {return "ZCurve cannot be constructed with negative cell bounds.\n";};
};

struct ZCurveTooLargeException : std::exception {
  const char *what() const noexcept {return "ZCurve cannot use such high indices";};
};

class ZCurve {
public:
  ZCurve();
  unsigned long initWithMax(const glm::ivec3 &cellBounds);
  unsigned long getIndex(const glm::ivec3 &p);
  unsigned long getIndex(unsigned int i, unsigned int j, unsigned int k);
  unsigned long splitBits(unsigned long i);

  #if MFluidSolver_ZCURVE_CACHING
  unsigned long calculateIndex(const glm::ivec3 &p);
  unsigned long calculateIndex(unsigned int i, unsigned int j, unsigned int k);
  #endif

private:
  unsigned long maxIndex;

  #if MFluidSolver_ZCURVE_CACHING
  std::vector<std::vector<std::vector<unsigned long>>> cache;
  #endif
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_ZCURVE_HPP_ */