//
//  sphParticle.cpp
//  MFluidSolver

#include "sphParticle.hpp"

SPHParticle::SPHParticle(float mass, const glm::vec3 &position)
: Particle(mass, position),
  _density(1000.f), _pressure(0.f),
  _nonPressureForce(0), _pressureForce(0), _oldPosition(position),
  _advectionDiagonal(0.f), _advectionDisplacementEstimate(0), _sumPressureDisplacementFromNeighbors(0),
  _densityIntermediate(1000.f) {
  #if MFluidSolver_PARTICLE_STATS
  flyaway = false;
  #endif
}

bool SPHParticle::indexCompare(const SPHParticle &a, const SPHParticle &b) {
  return a.index < b.index;
}
