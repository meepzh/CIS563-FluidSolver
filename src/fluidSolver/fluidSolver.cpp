//
//  fluidSolver.cpp
//  MFluidSolver

#include "fluidSolver.hpp"

FluidSolver::FluidSolver()
 : maxParticles(10000), _gravity(-9.8) {
}

FluidSolver::~FluidSolver() {
  for (Particle *p : particles) {
    delete p;
  }
}

void FluidSolver::update(double deltaT) {
  for (Particle *p : particles) {
    p->addForce(glm::vec3(0, _gravity, 0));
  }

  for (Particle *p : particles) {
    p->update(deltaT);
  }
}

void FluidSolver::setGravity(float g) {
  _gravity = g;
}
float FluidSolver::gravity() {
  return _gravity;
}