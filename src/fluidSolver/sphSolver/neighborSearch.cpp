//
//  neighborSearch.cpp
//  MFluidSolver

#include "neighborSearch.hpp"

void NaiveNeighborSearch::findNeighbors(Particle *p, std::vector<Particle *> &neighbors) {

}

void NaiveNeighborSearch::addParticle(Particle *p) {
  particleList.push_back(p);
}

StandardGridNeighborSearch::StandardGridNeighborSearch(const glm::vec3 &gridMin, const glm::vec3 &gridMax) {

}

void StandardGridNeighborSearch::findNeighbors(Particle *p, std::vector<Particle *> &neighbors) {

}

void StandardGridNeighborSearch::addParticle(Particle *p) {

}