//
//  fluidSolver.cpp
//  MFluidSolver

#include "fluidSolver.hpp"

FluidSolver::FluidSolver()
 : maxParticles(100000), _gravity(-9.8), _particleSeparation(0.1f),
  fluidSource(nullptr), fluidContainer(nullptr) {
  _particles = new std::vector<Particle *>();
}

FluidSolver::~FluidSolver() {
  for (Particle *p : *_particles) {
    delete p;
  }
  delete _particles;
}

void FluidSolver::update(double deltaT) {
  for (Particle *p : *_particles) {
    p->addForce(glm::vec3(0, _gravity, 0));
  }

  for (Particle *p : *_particles) {
    p->update(deltaT);
    if (!fluidContainer->intersects(p->position())) {
      p->color = glm::vec3(1, 0, 0);
    }
  }
}

void FluidSolver::addParticle(Particle *p) {
  if (_particles->size() < maxParticles) {
    _particles->push_back(p);
  }
}

const std::vector<Particle *> *FluidSolver::particles() const {
  return _particles;
}

unsigned int FluidSolver::numParticles() const {
  return _particles->size();
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