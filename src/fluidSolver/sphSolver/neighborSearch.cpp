//  Copyright 2016 Robert Zhou
//
//  neighborSearch.cpp
//  MFluidSolver

#include "neighborSearch.hpp"

#include <string>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/gtx/norm.hpp>

NeighborSearch::NeighborSearch(float r) {
  setSearchRadius(r);
}

void NeighborSearch::setSearchRadius(float r) {
  searchRadius = r;
  searchRadius2 = r * r;
}

void NeighborSearch::printPerformanceStats() {
}

void NaiveNeighborSearch::findNeighbors(SPHParticle *p) {
  const glm::vec3 pPos = p->position();
  std::vector<SPHParticle *> *neighbors = p->neighbors();
  neighbors->clear();

  // Filter neighbors by kernel radius
  for (SPHParticle *n : particleList) {
    if (n != p) {
      if (glm::distance2(n->position(), pPos) < searchRadius2) {
        neighbors->push_back(n);
      }
    }
  }
}

void NaiveNeighborSearch::addParticle(SPHParticle *p) {
  particleList.push_back(p);
}

void NaiveNeighborSearch::clear() {
  particleList.clear();
}

GridNeighborSearch::GridNeighborSearch(float r)
: NeighborSearch(r), grid(nullptr) {
}

GridNeighborSearch::~GridNeighborSearch() {
  delete grid;
}

void GridNeighborSearch::findNeighbors(SPHParticle *p) {
  grid->getNeighbors(p);

  const glm::vec3 pPos = p->position();
  std::vector<SPHParticle *> *neighbors = p->neighbors();

  // Filter neighbors by kernel radius. Also check for itself
  unsigned int numCandidates = neighbors->size();
  unsigned int idx = 0;
  unsigned int count = 0;
  SPHParticle *n;
  while (count < numCandidates) {
    n = (*neighbors)[idx];
    if (glm::distance2(n->position(), pPos) >= searchRadius2 || n == p) {
      neighbors->erase(neighbors->begin() + idx);
    } else {
      ++idx;
    }
    count++;
  }
}

void GridNeighborSearch::addParticle(SPHParticle *p) {
  grid->addParticle(p);
}

void GridNeighborSearch::updateParticle(SPHParticle *p) {
  grid->updateParticle(p);
}

void GridNeighborSearch::clear() {
  grid->clear();
}

void GridNeighborSearch::printDiagnostics() {
  grid->printDiagnostics();
}

#if MFluidSolver_USE_OPENVDB
void GridNeighborSearch::exportVDB() {
  std::string filename = "export.vdb";
  std::string exportname = "MExport";
  grid->exportVDB(filename, exportname);
}
#endif

UniformGridNeighborSearch::UniformGridNeighborSearch(
  float r, const glm::vec3 &gridMin, const glm::vec3 &gridMax, float cellSize)
    : GridNeighborSearch(r) {
  grid = new SPHUniformGrid(gridMin, gridMax, cellSize);
}

IndexSortedUniformGridNeighborSearch::IndexSortedUniformGridNeighborSearch(
  float r, const glm::vec3 &gridMin, const glm::vec3 &gridMax, float cellSize,
  std::vector<SPHParticle> *master, bool useZCurve)
    : GridNeighborSearch(r) {
  // Store a pointer to the grid so we don't need to cast
  if (useZCurve) {
    isuGrid =
      new SPHZIndexSortedUniformGrid(gridMin, gridMax, cellSize, master);
  } else {
    isuGrid =
      new SPHIndexSortedUniformGrid(gridMin, gridMax, cellSize, master);
  }
  grid = isuGrid;
}
