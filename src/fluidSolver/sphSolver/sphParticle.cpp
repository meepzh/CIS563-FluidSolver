//
//  sphParticle.cpp
//  MFluidSolver

#include "sphParticle.hpp"

SPHParticle::SPHParticle(const glm::vec3 &position)
: Particle(position),
  _density(1000.f), _pressure(1000.f), _forceDensity(glm::vec3(0)) {
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

std::vector<Particle *> &SPHParticle::neighbors() {
  return _neighbors;
}
