//  Copyright 2016 Robert Zhou
//
//  sphGrid.cpp
//  MFluidSolver

#include "sphGrid.hpp"

#include <iostream>

SPHGrid::SPHGrid(
  const glm::vec3 &minBounds, const glm::vec3 &maxBounds, float cellSize)
    : minBounds(minBounds), maxBounds(maxBounds), cellSize(cellSize) {
  glm::vec3 gridSize = maxBounds - minBounds;
  // Check grid bounds for sanity
  if (gridSize.x < 0 || gridSize.y < 0 || gridSize.z < 0) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_FATAL
    std::cerr << "FATAL: Grid size (" <<
                  gridSize.x << ", " <<
                  gridSize.y << ", " <<
                  gridSize.z << ") is invalid" << std::endl;
    #endif
    throw SPHGridNegativeSizeException();
  }

  // Calculate grid info
  cellBounds = (gridSize + 0.500001f) / cellSize;
  numCells = cellBounds.x * cellBounds.y * cellBounds.z;
}

glm::ivec3 SPHGrid::getGridCoordinates(const glm::vec3 &pt) {
  #if MFluidSolver_USE_ASSERTS
  assert(pt.x >= minBounds.x && pt.y >= minBounds.y && pt.z >= minBounds.z);
  assert(pt.x <= maxBounds.x && pt.y <= maxBounds.y && pt.z <= maxBounds.z);
  #endif
  return glm::ivec3(static_cast<int>((pt.x - minBounds.x) / cellSize),
                    static_cast<int>((pt.y - minBounds.y) / cellSize),
                    static_cast<int>((pt.z - minBounds.z) / cellSize));
}

uint32_t SPHGrid::getIndex(const glm::vec3 &pt) {
  return getIndex(getGridCoordinates(pt));
}

uint32_t SPHGrid::getIndex(const glm::ivec3 &c) {
  #if MFluidSolver_USE_ASSERTS
  assert(c.x >= 0 && c.y >= 0 && c.z >= 0);
  assert(c.x < cellBounds.x && c.y < cellBounds.y && c.z < cellBounds.z);
  #endif

  return getIndex(c.x, c.y, c.z);
}

uint32_t SPHGrid::getIndex(
    unsigned int x, unsigned int y, unsigned int z) {
  #if MFluidSolver_USE_ASSERTS
  uint32_t idx = y * cellBounds.z * cellBounds.x + z * cellBounds.x + x;
  assert(idx >= 0 && idx < numCells);
  return idx;
  #else
  return y * cellBounds.z * cellBounds.x + z * cellBounds.x + x;
  #endif
}
