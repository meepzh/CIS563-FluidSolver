//
//  sphZIndexSortedUniformGrid.cpp
//  MFluidSolver

#include "sphZIndexSortedUniformGrid.hpp"

SPHZIndexSortedUniformGrid(const glm::vec3 &minBounds, const glm::vec3 &maxBounds, float cellSize, std::vector<SPHParticle> *master)
 : SPHGrid(minBounds, maxBounds, cellSize), master(master){
  cells.resize(numCells);
}

~SPHZIndexSortedUniformGrid() {

}

void addParticle(SPHParticle *p) {

}

void updateParticle(SPHParticle *p) {

}

void getNeighbors(SPHParticle *p) {

}

void clear() {

}

glm::ivec3 getGridCoordinates(const glm::vec3 &pt) {

}

unsigned int getIndex(const glm::ivec3 &c) {

}

unsigned int getIndex(unsigned int x, unsigned int y, unsigned int z) {

}

void printDiagnostics() {

}

#if MFluidSolver_USE_OPENVDB
void exportVDB(std::string &file, std::string &gridName) {

}
#endif