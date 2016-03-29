//
//  fluidSolver.cpp
//  MFluidSolver

#include "fluidSolver.hpp"

FluidSolver::FluidSolver()
 : _maxParticles(MFluidSolver_DEFAULT_MAX_PARTICLES), _gravity(MFluidSolver_DEFAULT_GRAVITY),
   _particleSeparation(MFluidSolver_DEFAULT_PARTICLE_SEPARATION),
   visualizationType(MFluidSolver_DEFAULT_VISUALIZATION),
   _fixedTimestep(MFluidSolver_DEFAULT_UPDATE_STEP),
   fluidSource(nullptr), fluidContainer(nullptr) {

}

FluidSolver::~FluidSolver() {

}

void FluidSolver::update(double deltaT) {

}

void FluidSolver::updateStep() {
  update(_fixedTimestep);
}

void FluidSolver::setGravity(float g) {
  _gravity = g;
}
float FluidSolver::gravity() const {
  return _gravity;
}

void FluidSolver::setMaxParticles(int mp) {
  _maxParticles = mp;
}
int FluidSolver::maxParticles() const {
  return _maxParticles;
}

void FluidSolver::setParticleSeparation(float ps) {
  _particleSeparation = ps;
}
float FluidSolver::particleSeparation() const {
  return _particleSeparation;
}

void FluidSolver::setFixedTimestep(float ft) {
  _fixedTimestep = ft;
}