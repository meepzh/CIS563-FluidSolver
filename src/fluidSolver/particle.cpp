//
//  particle.cpp
//  MFluidSolver

#include "particle.hpp"

#if MFluidSolver_PARTICLE_STATS
unsigned int Particle::lastParticleID = 1;
#endif

// Constructors / Destructors
Particle::Particle(float mass, const glm::vec3 &position)
 : color(MFluidSolver_DEFAULT_PARTICLE_COLOR_R, MFluidSolver_DEFAULT_PARTICLE_COLOR_G, MFluidSolver_DEFAULT_PARTICLE_COLOR_B),
   _mass(mass), _position(position), _velocity(glm::vec3(0)) {
  #if MFluidSolver_PARTICLE_STATS
  // Note assumes atomic access to lastParticleID
  ID = lastParticleID;
  ++lastParticleID;
  #endif
}

// Operators
bool Particle::operator==(const Particle &b) const {
  return b.mass() == _mass && b.position() == _position && b.velocity() == _velocity;
}