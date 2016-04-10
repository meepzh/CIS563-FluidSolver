//
//  sphParticle.cpp
//  MFluidSolver

#include "sphParticle.hpp"

SPHParticle::SPHParticle(float mass, const glm::vec3 &position)
: Particle(mass, position),
  _density(1000.f), _pressure(1000.f), _forceDensity(glm::vec3(0)), _oldPosition(position) {
}

void SPHParticle::update(const glm::vec3 &newVel, const glm::vec3 &newPos) {
  _oldPosition = _position;
  _velocity = newVel;
  _position = newPos;
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

glm::vec3 SPHParticle::oldPosition() const {
  return _oldPosition;
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

bool SPHParticle::indexCompare(const SPHParticle &a, const SPHParticle &b) {
  return a.index < b.index;
}
