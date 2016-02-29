//
//  fluidSolver.cpp
//  MFluidSolver

#include "fluidSolver.hpp"

FluidSolver::~FluidSolver() {
  for (std::vector<Particle *>::iterator it = particles.begin(); it != particles.end(); ++it) {
    delete *it;
  }
}

void FluidSolver::update() {
  for (std::vector<Particle *>::iterator it = particles.begin(); it != particles.end(); ++it) {
    (*it)->addForce(glm::vec3(0, _gravity, 0));
  }

  for (std::vector<Particle *>::iterator it = particles.begin(); it != particles.end(); ++it) {
    (*it)->update();
  }
}

void FluidSolver::setGravity(float g) {
  _gravity = g;
}
float FluidSolver::gravity() {
  return _gravity;
}