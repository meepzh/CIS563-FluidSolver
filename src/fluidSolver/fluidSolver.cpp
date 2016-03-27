//
//  fluidSolver.cpp
//  MFluidSolver

#include "fluidSolver.hpp"

FluidSolver::FluidSolver()
 : maxParticles(FLUID_MAX_PARTICLES), _gravity(-9.8), _particleSeparation(0.1f),
  fluidSource(nullptr), fluidContainer(nullptr) {

}

FluidSolver::~FluidSolver() {

}

void FluidSolver::update(double deltaT) {

}

void FluidSolver::updateStep() {
  update(UPDATE_STEP_DEFAULT);
}

void FluidSolver::setGravity(float g) {
  _gravity = g;
}
float FluidSolver::gravity() const {
  return _gravity;
}

void FluidSolver::setParticleSeparation(float ps) {
  _particleSeparation = ps;
}
float FluidSolver::particleSeparation() const {
  return _particleSeparation;
}