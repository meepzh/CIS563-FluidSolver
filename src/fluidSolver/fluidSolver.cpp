//
//  fluidSolver.cpp
//  MFluidSolver

#include "fluidSolver.hpp"

FluidSolver::FluidSolver()
 : maxParticles(MFluidSolver_DEFAULT_MAX_PARTICLES), _gravity(MFluidSolver_DEFAULT_GRAVITY),
   _particleSeparation(MFluidSolver_DEFAULT_PARTICLE_SEPARATION),
   fluidSource(nullptr), fluidContainer(nullptr) {

}

FluidSolver::~FluidSolver() {

}

void FluidSolver::update(double deltaT) {

}

void FluidSolver::updateStep() {
  update(MFluidSolver_DEFAULT_UPDATE_STEP);
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