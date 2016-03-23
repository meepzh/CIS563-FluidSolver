//
//  neighborSearch.cpp
//  MFluidSolver

#include "neighborSearch.hpp"

#define GLM_FORCE_RADIANS
#include <glm/gtx/norm.hpp>

NeighborSearch::NeighborSearch(float r) {
  setSearchRadius(r);
}

void NeighborSearch::setSearchRadius(float r) {
  searchRadius = r;
  searchRadius2 = r * r;
}

void NaiveNeighborSearch::findNeighbors(SPHParticle *p) {
  const glm::vec3 pPos = p->position();
  std::vector<SPHParticle *> &neighbors = p->neighbors();
  for (SPHParticle *n : particleList) {
    if (n != p) {
      if (glm::distance2(n->position(), pPos) < searchRadius2) {
        neighbors.push_back(n);
      }
    }
  }
}

void NaiveNeighborSearch::addParticle(SPHParticle *p) {
  particleList.push_back(p);
}

StandardGridNeighborSearch::StandardGridNeighborSearch(float r, const glm::vec3 &gridMin, const glm::vec3 &gridMax, float cellSize)
: NeighborSearch(r), grid(nullptr) {
  grid = new SPHGrid(gridMin, gridMax, cellSize);
}

StandardGridNeighborSearch::~StandardGridNeighborSearch() {
  delete grid;
}

void StandardGridNeighborSearch::findNeighbors(SPHParticle *p) {
  grid->getNeighbors(p);

  const glm::vec3 pPos = p->position();
  std::vector<SPHParticle *> &neighbors = p->neighbors();

  unsigned int numCandidates = neighbors.size();
  unsigned int i = 0;
  unsigned int count = 0;
  SPHParticle *n;
  while (count < numCandidates) {
    n = neighbors.at(i);
    if (glm::distance2(n->position(), pPos) >= searchRadius2 || n == p) {
      neighbors.erase(neighbors.begin() + i);
    } else {
      ++i;
    }
    count++;
  }
}

void StandardGridNeighborSearch::addParticle(SPHParticle *p) {
  grid->addParticle(p);
}

void StandardGridNeighborSearch::clear() {
  grid->clear();
}
