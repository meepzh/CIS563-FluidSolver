//
//  sphSolver.cpp
//  MFluidSolver

#include "sphSolver.hpp"

SPHSolver::SPHSolver()
 : nSearch(nullptr) {
}
SPHSolver::~SPHSolver() {
  delete nSearch;
}

SPHConfig *SPHSolver::init(const double &kernelRadius,
    const glm::vec3 &gridMin, const glm::vec3 &gridMax, NeighborSearchType nSearchType) {
  kernelFunctions.setKernelRadius(kernelRadius);

  switch (nSearchType) {
    case NeighborSearchType::Naive:
      nSearch = new NaiveNeighborSearch();
      break;
    case NeighborSearchType::StandardGrid:
    default:
      nSearch = new StandardGridNeighborSearch(kernelRadius, gridMin, gridMax);
      break;
  }

  return &config;
}

void SPHSolver::setDefaultConfig() {

}

void SPHSolver::update(double deltaT) {

}

void SPHSolver::addParticle(Particle *p) {
  nSearch->addParticle(p);
}

void SPHSolver::setParticleSeparation(float ps) {

}