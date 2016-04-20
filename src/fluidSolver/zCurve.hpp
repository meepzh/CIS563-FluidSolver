//  Copyright 2016 Robert Zhou
//
//  zCurve.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_ZCURVE_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_ZCURVE_HPP_

#include <exception>
#include <glm/glm.hpp>

#include "MFluidSolverConfig.hpp"

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
  const char *what() const noexcept {
    return "ZCurve cannot be constructed with negative cell bounds.\n";
  };
};

struct ZCurveTooLargeException : std::exception {
  const char *what() const noexcept {
    return "ZCurve cannot use such high indices";
  };
};

class ZCurve {
 public:
  ZCurve();
  uint32_t initWithMax(const glm::ivec3 &cellBounds);
  uint32_t getIndex(const glm::ivec3 &p);
  uint32_t getIndex(unsigned int i, unsigned int j, unsigned int k);
  uint32_t splitBits(uint32_t i);

  #if MFluidSolver_ZCURVE_CACHING
  uint32_t calculateIndex(const glm::ivec3 &p);
  uint32_t calculateIndex(unsigned int i, unsigned int j, unsigned int k);
  #endif

 private:
  uint32_t maxIndex;

  #if MFluidSolver_ZCURVE_CACHING
  std::vector<std::vector<std::vector<uint32_t>>> cache;
  #endif
};

#endif  // MFLUIDSOLVER_FLUIDSOLVER_ZCURVE_HPP_
