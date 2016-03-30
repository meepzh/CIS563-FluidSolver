//
//  neighborSearch.cpp
//  MFluidSolver

#include "neighborSearch.hpp"

#define GLM_FORCE_RADIANS
#include <glm/gtx/norm.hpp>

#if MFluidSolver_RECORD_PERFORMANCE
#include <ctime>
#include "../../utils.hpp"
#endif

NeighborSearch::NeighborSearch(float r)
 : computeTime(0), numSearches(0) {
  setSearchRadius(r);
}

void NeighborSearch::setSearchRadius(float r) {
  searchRadius = r;
  searchRadius2 = r * r;
}

void NeighborSearch::printPerformanceStats() {
  #if MFluidSolver_RECORD_PERFORMANCE
  std::cout << "PERF: Neighbor search averaged " <<
    (computeTime / (double)numSearches) <<
    " seconds over " << numSearches << " searches" << std::endl;
  std::cout << "PERF: Overall search time was " << MUtils::toHMS(computeTime) << std::endl;
  #endif
}

void NaiveNeighborSearch::findNeighbors(SPHParticle *p) {
  #if MFluidSolver_RECORD_PERFORMANCE
  std::clock_t startTime = std::clock();
  #endif

  const glm::vec3 pPos = p->position();
  std::vector<SPHParticle *> *neighbors = p->neighbors();
  neighbors->clear();

  for (SPHParticle *n : particleList) {
    if (n != p) {
      if (glm::distance2(n->position(), pPos) < searchRadius2) {
        neighbors->push_back(n);
      }
    }
  }

  #if MFluidSolver_RECORD_PERFORMANCE
  std::clock_t endTime = std::clock();
  double computeTimeInSeconds = (endTime - startTime) / (double) CLOCKS_PER_SEC;
  ++numSearches;
  computeTime += computeTimeInSeconds;
  #endif
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
  #if MFluidSolver_RECORD_PERFORMANCE
  std::clock_t startTime = std::clock();
  #endif

  grid->getNeighbors(p);

  const glm::vec3 pPos = p->position();
  std::vector<SPHParticle *> *neighbors = p->neighbors();

  unsigned int numCandidates = neighbors->size();
  unsigned int i = 0;
  unsigned int count = 0;
  SPHParticle *n;
  while (count < numCandidates) {
    n = neighbors->at(i);
    if (glm::distance2(n->position(), pPos) >= searchRadius2 || n == p) {
      neighbors->erase(neighbors->begin() + i);
    } else {
      ++i;
    }
    count++;
  }

  #if MFluidSolver_RECORD_PERFORMANCE
  std::clock_t endTime = std::clock();
  double computeTimeInSeconds = (endTime - startTime) / (double) CLOCKS_PER_SEC;
  ++numSearches;
  computeTime += computeTimeInSeconds;
  #endif
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

UniformGridNeighborSearch::UniformGridNeighborSearch(float r, const glm::vec3 &gridMin, const glm::vec3 &gridMax, float cellSize)
 : GridNeighborSearch(r) {
  grid = new SPHUniformGrid(gridMin, gridMax, cellSize);
}

IndexSortedUniformGridNeighborSearch::IndexSortedUniformGridNeighborSearch(
  float r, const glm::vec3 &gridMin, const glm::vec3 &gridMax, float cellSize,
  std::vector<SPHParticle> *master, bool useZCurve)
 : GridNeighborSearch(r) {
  if (useZCurve) {
    isuGrid = new SPHZIndexSortedUniformGrid(gridMin, gridMax, cellSize, master);
  } else {
    isuGrid = new SPHIndexSortedUniformGrid(gridMin, gridMax, cellSize, master);
  }
  grid = isuGrid;
}