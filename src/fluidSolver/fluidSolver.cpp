//
//  fluidSolver.cpp
//  MFluidSolver

#include "fluidSolver.hpp"

#include <iostream>

#include "../utils.hpp"

FluidSolver::FluidSolver()
 : _maxParticles(MFluidSolver_DEFAULT_MAX_PARTICLES), _gravity(MFluidSolver_DEFAULT_GRAVITY),
   _particleSeparation(MFluidSolver_DEFAULT_PARTICLE_SEPARATION),
   visualizationType(MFluidSolver_DEFAULT_VISUALIZATION),
   _fixedTimestep(MFluidSolver_DEFAULT_UPDATE_STEP),
   computeTime(0), numUpdates(0), firstRun(true), endedSimulation(false),
   maxUpdates(MFluidSolver_DEFAULT_MAX_UPDATES), limitNumUpdates(MFluidSolver_DEFAULT_LIMIT_UPDATES),
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

bool FluidSolver::hasEndedSimulation() {
  return endedSimulation;
}

void FluidSolver::endSimulation() {
  if (!endedSimulation) {
    endTime = tbb::tick_count::now();
    endedSimulation = true;
  }
}

void FluidSolver::printPerformanceStats() {
  computeTime = (endTime - startTime).seconds();
  std::cout << "PERF: Overall simulation ran for " << MUtils::toHMS(computeTime) << " over " << numUpdates << " frames (" << computeTime << " seconds)" << std::endl;
}