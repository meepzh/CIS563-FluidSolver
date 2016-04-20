//  Copyright 2016 Robert Zhou
//
//  zCurve.cpp
//  MFluidSolver

#include "zCurve.hpp"

#include <iostream>
#include <vector>

#if MFluidSolver_USE_ASSERTS
#include <cassert>
#endif

#define LONG_COORD_LIMIT 2047

ZCurve::ZCurve() {
}

uint32_t ZCurve::initWithMax(const glm::ivec3 &cellBounds) {
  // Check sane bounds
  if (cellBounds.x < 0 || cellBounds.y < 0 || cellBounds.z < 0) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_FATAL
    std::cerr << "FATAL: ZCurve bounds (" <<
                  cellBounds.x << ", " <<
                  cellBounds.y << ", " <<
                  cellBounds.z << ") is negative" << std::endl;
    #endif
    throw ZCurveNegativeBoundsException();
  }

  if (cellBounds.x > LONG_COORD_LIMIT ||
      cellBounds.y > LONG_COORD_LIMIT ||
      cellBounds.z > LONG_COORD_LIMIT) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_FATAL
    std::cerr << "FATAL: ZCurve bounds (" <<
                  cellBounds.x << ", " <<
                  cellBounds.y << ", " <<
                  cellBounds.z << ") exceeds limit " <<
                  LONG_COORD_LIMIT << std::endl;
    #endif
    throw ZCurveTooLargeException();
  }

  // Cache Z-index if asked
  #if MFluidSolver_ZCURVE_CACHING
  maxIndex = calculateIndex(cellBounds);
  #else
  maxIndex = getIndex(cellBounds);
  #endif

  #if MFluidSolver_ZCURVE_CACHING
  for (unsigned int i = 0; i < cellBounds.x; ++i) {
    cache.push_back(std::vector<std::vector<uint32_t>>());
    for (unsigned int j = 0; j < cellBounds.y; ++j) {
      cache[i].push_back(std::vector<uint32_t>());
      for (unsigned int k = 0; k < cellBounds.z; ++k) {
        cache[i][j].push_back(calculateIndex(i, j, k));
      }
    }
  }
  #endif

  return maxIndex;
}

#if MFluidSolver_ZCURVE_CACHING
uint32_t ZCurve::getIndex(const glm::ivec3 &p) {
  #if MFluidSolver_USE_ASSERTS
  assert(p.x >= 0 && p.y >= 0 && p.z >= 0);
  assert(p.x < cache.size() &&
         p.y < cache[0].size() &&
         p.z < cache[0][0].size());
  #endif
  return cache[p.x][p.y][p.z];
}

uint32_t ZCurve::getIndex(unsigned int i, unsigned int j, unsigned int k) {
  #if MFluidSolver_USE_ASSERTS
  assert(i <= LONG_COORD_LIMIT &&
         j <= LONG_COORD_LIMIT &&
         k <= LONG_COORD_LIMIT);
  #endif

  return getIndex(glm::ivec3(i, j, k));
}

uint32_t ZCurve::calculateIndex(const glm::ivec3 &p) {
  #if MFluidSolver_USE_ASSERTS
  assert(p.x >= 0 && p.y >= 0 && p.z >= 0);
  #endif

  return calculateIndex(p.x, p.y, p.z);
}
#else
uint32_t ZCurve::getIndex(const glm::ivec3 &p) {
  #if MFluidSolver_USE_ASSERTS
  assert(p.x >= 0 && p.y >= 0 && p.z >= 0);
  #endif

  return getIndex(p.x, p.y, p.z);
}
#endif

#if MFluidSolver_ZCURVE_CACHING
uint32_t ZCurve::calculateIndex(
  unsigned int i, unsigned int j, unsigned int k) {
#else
uint32_t ZCurve::getIndex(unsigned int i, unsigned int j, unsigned int k) {
#endif
  return (splitBits(k) << 2) | (splitBits(j) << 1) | splitBits(i);
}

// http://stackoverflow.com/questions/1024754/how-to-compute-a-3d-morton-number-interleave-the-bits-of-3-ints
uint32_t ZCurve::splitBits(uint32_t i) {
  i = (i | (i << 16)) & 0x030000FF;
  i = (i | (i <<  8)) & 0x0300F00F;
  i = (i | (i <<  4)) & 0x030C30C3;
  i = (i | (i <<  2)) & 0x09249249;
  return i;
}
