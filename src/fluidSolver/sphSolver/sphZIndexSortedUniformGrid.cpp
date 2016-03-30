//
//  sphZIndexSortedUniformGrid.cpp
//  MFluidSolver

#include "sphZIndexSortedUniformGrid.hpp"

SPHZIndexSortedUniformGrid::SPHZIndexSortedUniformGrid(const glm::vec3 &minBounds, const glm::vec3 &maxBounds, float cellSize, std::vector<SPHParticle> *master)
 : SPHIndexSortedUniformGrid(minBounds, maxBounds, cellSize, master) {
  numCells = zCurve.maxIndex(cellBounds);
  cells.resize(numCells);
}

SPHZIndexSortedUniformGrid::~SPHZIndexSortedUniformGrid() {

}

unsigned long SPHZIndexSortedUniformGrid::getIndex(unsigned int x, unsigned int y, unsigned int z) {
  return zCurve.getIndex(x, y, z);
}