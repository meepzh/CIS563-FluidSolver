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

void NaiveNeighborSearch::findNeighbors(Particle *p, std::vector<Particle *> &neighbors) {
  const glm::vec3 pPos = p->position();
  for (Particle *n : particleList) {
    if (n != p) {
      if (glm::distance2(n->position(), pPos) < searchRadius2) {
        neighbors.push_back(n);
      }
    }
  }
}

void NaiveNeighborSearch::addParticle(Particle *p) {
  particleList.push_back(p);
}

void NaiveNeighborSearch::updateParticle(Particle *p) {
  // Do nothing
}

StandardGridNeighborSearch::StandardGridNeighborSearch(float r, const glm::vec3 &gridMin, const glm::vec3 &gridMax)
: NeighborSearch(r) {

}

void StandardGridNeighborSearch::findNeighbors(Particle *p, std::vector<Particle *> &neighbors) {

}

void StandardGridNeighborSearch::addParticle(Particle *p) {

}

void StandardGridNeighborSearch::updateParticle(Particle *p) {

}
