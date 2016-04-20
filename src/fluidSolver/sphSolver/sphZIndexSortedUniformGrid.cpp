//  Copyright 2016 Robert Zhou
//
//  sphZIndexSortedUniformGrid.cpp
//  MFluidSolver

#include <vector>

#include "sphZIndexSortedUniformGrid.hpp"

SPHZIndexSortedUniformGrid::SPHZIndexSortedUniformGrid(
  const glm::vec3 &minBounds, const glm::vec3 &maxBounds,
  float cellSize, std::vector<SPHParticle> *master)
    : SPHIndexSortedUniformGrid(minBounds, maxBounds, cellSize, master) {
  numCells = zCurve.initWithMax(cellBounds);
  cells.resize(numCells);
}

SPHZIndexSortedUniformGrid::~SPHZIndexSortedUniformGrid() {
}

uint32_t SPHZIndexSortedUniformGrid::getIndex(
    unsigned int x, unsigned int y, unsigned int z) {
  return zCurve.getIndex(x, y, z);
}
