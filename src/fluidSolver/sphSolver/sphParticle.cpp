//
//  sphParticle.cpp
//  MFluidSolver

#include "sphParticle.hpp"

SPHParticle::SPHParticle(float mass, const glm::vec3 &position)
: Particle(mass, position),
  _density(1000.f), _pressure(1000.f),
  _nonPressureForce(0), _pressureForce(0), _oldPosition(position),
  _aSelf(0.f), _dSelf(0), _dNeighbors(0) {
}

bool SPHParticle::indexCompare(const SPHParticle &a, const SPHParticle &b) {
  return a.index < b.index;
}
