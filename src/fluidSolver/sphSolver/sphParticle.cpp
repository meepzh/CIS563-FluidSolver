//
//  sphParticle.cpp
//  MFluidSolver

#include "sphParticle.hpp"

SPHParticle::SPHParticle(float mass, const glm::vec3 &position)
: Particle(mass, position),
  _density(1000.f), _pressure(1000.f),
  _nonPressureForce(glm::vec3(0)), _pressureForce(glm::vec3(0)), _oldPosition(position) {
}

bool SPHParticle::indexCompare(const SPHParticle &a, const SPHParticle &b) {
  return a.index < b.index;
}
