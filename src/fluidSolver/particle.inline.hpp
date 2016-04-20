//  Copyright 2016 Robert Zhou
//
//  particle.inline.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_INLINE_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_INLINE_HPP_

// Update
inline void Particle::setPosition(const glm::vec3 &position) {
  _position = position;
}

inline void Particle::reverseVelocity(
    const glm::ivec3 &directions, float bounceCoefficient) {
  if (directions.x != 0) _velocity.x *= -1 * bounceCoefficient;
  if (directions.y != 0) _velocity.y *= -1 * bounceCoefficient;
  if (directions.z != 0) _velocity.z *= -1 * bounceCoefficient;
}

inline void Particle::stopVelocity(const glm::ivec3 &directions) {
  if (directions.x != 0) _velocity.x = 0;
  if (directions.y != 0) _velocity.y = 0;
  if (directions.z != 0) _velocity.z = 0;
}

// Properties
inline float Particle::mass() const {
  return _mass;
}

inline glm::vec3 Particle::position() const {
  return _position;
}

inline glm::vec3 Particle::velocity() const {
  return _velocity;
}

#endif  // MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_INLINE_HPP_
