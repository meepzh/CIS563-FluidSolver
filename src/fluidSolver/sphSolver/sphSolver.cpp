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

SPHConfig *SPHSolver::init(const double &kernelRadius, const glm::vec3 &gridMin, const glm::vec3 &gridMax) {
  return &config;
}

void SPHSolver::setDefaultConfig() {

}