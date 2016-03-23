//
//  sphGrid.cpp
//  MFluidSolver

#include "sphGrid.hpp"
#include <assert.h>

SPHGrid::SPHGrid(const glm::vec3 &minBounds, const glm::vec3 &maxBounds, float cellSize)
 : minBounds(minBounds), maxBounds(maxBounds), cellSize(cellSize) {
  glm::vec3 gridSize = maxBounds - minBounds;
  assert(gridSize.x > 0 && gridSize.y > 0 && gridSize.z > 0);

  cellBounds = (gridSize + 0.5f) / cellSize;

  data = new std::vector<std::vector<SPHParticle *>>();
  numCells = cellBounds.x * cellBounds.y * cellBounds.z;
  data->resize(numCells);
}

SPHGrid::~SPHGrid() {
  delete data;
}

void SPHGrid::addParticle(SPHParticle *p) {
  glm::ivec3 pC = getGridCoordinates(p->position());
  unsigned int pI = getIndex(pC);
  data->at(pI).push_back(p);
}

void SPHGrid::getNeighbors(SPHParticle *p) {
  glm::ivec3 pC = getGridCoordinates(p->position());
  std::vector<SPHParticle *> &neighbors = p->neighbors();

  unsigned int index = getIndex(pC);
  std::vector<SPHParticle *> &vec = data->at(index);
  // Add all but original particle in current cell (best place to do this is here)
  for (unsigned int i = 0; i < vec.size(); ++i) {
    if (vec.at(i) != p) {
      neighbors.push_back(vec.at(i));
    }
  }

  glm::ivec3 coords;
  for (unsigned int i = -1; i < 2; ++i) {
    for (unsigned int j = -1; j < 2; ++j) {
      for (unsigned int k = -1; k < 2; ++k) {
        coords = glm::ivec3(pC.x + i, pC.y + j, pC.z + k);
        if (coords != pC) {
          // If not at original cell, add all neighbors (process later in kernel)
          vec = data->at(index);
          for (unsigned int l = 0; l < vec.size(); ++l) {
            neighbors.push_back(vec.at(l));
          } // end for l
        } // end if
      } // end for k
    } // end for j
  } // end for i
}

void SPHGrid::clear() {
  for (unsigned int i = 0; i < numCells; ++i) {
    data->at(i).clear();
  }
}

glm::ivec3 SPHGrid::getGridCoordinates(const glm::vec3 &pt) {
  return glm::ivec3((int)(pt.x / cellSize), (int)(pt.y / cellSize), (int)(pt.z / cellSize));
}

unsigned int SPHGrid::getIndex(unsigned int x, unsigned int y, unsigned int z) {
  return y * cellBounds.z * cellBounds.x + z * cellBounds.x + x;
}

unsigned int SPHGrid::getIndex(const glm::ivec3 &c) {
  return getIndex(c.x, c.y, c.z);
}