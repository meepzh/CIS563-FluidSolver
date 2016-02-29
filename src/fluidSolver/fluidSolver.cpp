//
//  fluidSolver.cpp
//  MFluidSolver

#include "fluidSolver.hpp"

FluidSolver::~FluidSolver() {
  for (Particle *p : particles) {
    delete p;
  }
}

void FluidSolver::update() {
  for (Particle *p : particles) {
    p->addForce(glm::vec3(0, _gravity, 0));
  }

  for (Particle *p : particles) {
    p->update();
  }
}

void FluidSolver::setGravity(float g) {
  _gravity = g;
}
float FluidSolver::gravity() {
  return _gravity;
}