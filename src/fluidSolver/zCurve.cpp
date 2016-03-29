//
//  zCurve.cpp
//  MFluidSolver

#include "zCurve.hpp"

#include <cassert>
#include <iostream>

#define LONG_COORD_LIMIT 2047

unsigned long ZCurve::maxIndex(const glm::ivec3 &cellBounds) {
  if (cellBounds.x < 0 || cellBounds.y < 0 || cellBounds.z < 0) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_FATAL
    std::cerr << "FATAL: ZCurve bounds (" << cellBounds.x << ", " << cellBounds.y << ", " << cellBounds.z << ") is negative" << std::endl;
    #endif
    throw ZCurveNegativeBoundsException();
  }

  if (cellBounds.x > LONG_COORD_LIMIT || cellBounds.y > LONG_COORD_LIMIT || cellBounds.z > LONG_COORD_LIMIT) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_FATAL
    std::cerr << "FATAL: ZCurve bounds (" << cellBounds.x << ", " << cellBounds.y << ", " << cellBounds.z << ") exceeds limit " << LONG_COORD_LIMIT << std::endl;
    #endif
    throw ZCurveTooLargeException();
  }

  return getIndex(cellBounds);
}

unsigned long ZCurve::getIndex(const glm::ivec3 &p) {
  return getIndex(p.x, p.y, p.z);
}

unsigned long ZCurve::getIndex(int i, int j, int k) {
  #if MFluidSolver_USE_ASSERTS
  assert(i >= 0 && j >= 0 && k >= 0);
  assert(i <= LONG_COORD_LIMIT && j <= LONG_COORD_LIMIT && k <= LONG_COORD_LIMIT);
  #endif

  return (splitBits(k) << 2) | (splitBits(j) << 1) | splitBits(i);
}

// http://stackoverflow.com/questions/1024754/how-to-compute-a-3d-morton-number-interleave-the-bits-of-3-ints
unsigned long ZCurve::splitBits(unsigned long i) {
  i = (i | (i << 16)) & 0x030000FF;
  i = (i | (i <<  8)) & 0x0300F00F;
  i = (i | (i <<  4)) & 0x030C30C3;
  i = (i | (i <<  2)) & 0x09249249;
}