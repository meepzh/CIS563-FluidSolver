//
//  sphParticle.cpp
//  MFluidSolver

#include "sphParticle.hpp"

SPHParticle::SPHParticle(float mass, const glm::vec3 &position)
: Particle(mass, position),
  _density(1000.f), _pressure(1000.f), _forceDensity(glm::vec3(0)) {
}

void SPHParticle::update(double deltaT) {
  _velocity += _forceDensity / _density * (float)deltaT;
  _position += _velocity * (float)deltaT;
}

float SPHParticle::density() const {
  return _density;
}

float SPHParticle::pressure() const {
  return _pressure;
}

glm::vec3 SPHParticle::forceDensity() const {
  return _forceDensity;
}

void SPHParticle::setDensity(float density) {
  _density = density;
}

void SPHParticle::setPressure(float pressure) {
  _pressure = pressure;
}

void SPHParticle::setForceDensity(const glm::vec3 &forceDensity) {
  _forceDensity = forceDensity;
}

void SPHParticle::addForceDensity(const glm::vec3 &forceDensity) {
  _forceDensity += forceDensity;
}

std::vector<SPHParticle *> *SPHParticle::neighbors() {
  return &_neighbors;
}

void SPHParticle::clearNeighbors() {
  _neighbors.clear();
}
