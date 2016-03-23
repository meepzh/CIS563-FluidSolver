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

void NaiveNeighborSearch::updateParticle(SPHParticle *p) {
  // Do nothing
}

StandardGridNeighborSearch::StandardGridNeighborSearch(float r, const glm::vec3 &gridMin, const glm::vec3 &gridMax)
: NeighborSearch(r) {

}

void StandardGridNeighborSearch::findNeighbors(SPHParticle *p) {

}

void StandardGridNeighborSearch::addParticle(SPHParticle *p) {

}

void StandardGridNeighborSearch::updateParticle(SPHParticle *p) {

}
