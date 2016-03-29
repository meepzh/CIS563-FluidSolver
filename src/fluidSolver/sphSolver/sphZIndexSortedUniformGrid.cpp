//
//  sphZIndexSortedUniformGrid.cpp
//  MFluidSolver

#include "sphZIndexSortedUniformGrid.hpp"

SPHZIndexSortedUniformGrid::SPHZIndexSortedUniformGrid(const glm::vec3 &minBounds, const glm::vec3 &maxBounds, float cellSize, std::vector<SPHParticle> *master)
 : SPHGrid(minBounds, maxBounds, cellSize), master(master){
  cells.resize(numCells);
  zMax = zCurve.maxIndex(cellBounds);
}

SPHZIndexSortedUniformGrid::~SPHZIndexSortedUniformGrid() {

}

void SPHZIndexSortedUniformGrid::addParticle(SPHParticle *p) {

}

void SPHZIndexSortedUniformGrid::updateParticle(SPHParticle *p) {

}

void SPHZIndexSortedUniformGrid::getNeighbors(SPHParticle *p) {

}

void SPHZIndexSortedUniformGrid::clear() {

}

glm::ivec3 SPHZIndexSortedUniformGrid::getGridCoordinates(const glm::vec3 &pt) {
  return glm::ivec3(0);
}

unsigned int SPHZIndexSortedUniformGrid::getIndex(const glm::ivec3 &c) {
  return 0;
}

unsigned int SPHZIndexSortedUniformGrid::getIndex(unsigned int x, unsigned int y, unsigned int z) {
  return 0;
}

void SPHZIndexSortedUniformGrid::printDiagnostics() {

}

#if MFluidSolver_USE_OPENVDB
void SPHZIndexSortedUniformGrid::exportVDB(std::string &file, std::string &gridName) {

}
#endif