//
//  particle.cpp
//  MFluidSolver

#include "particle.hpp"

Particle::Particle(float mass, const glm::vec3 &position)
 : color(0, 0, 1), _mass(mass), _position(position), _velocity(glm::vec3(0)) {
}

void Particle::update(double deltaT) {
  _position += _velocity * (float)deltaT;
}

void Particle::reverseVelocity(const glm::ivec3 &directions) {
  if (directions.x != 0) _velocity.x *= -1;
  if (directions.y != 0) _velocity.y *= -1;
  if (directions.z != 0) _velocity.z *= -1;
}

void Particle::stopVelocity(const glm::ivec3 &directions) {
  if (directions.x != 0) _velocity.x = 0;
  if (directions.y != 0) _velocity.y = 0;
  if (directions.z != 0) _velocity.z = 0;
}

void Particle::setPosition(const glm::vec3 &position) {
  _position = position;
}

float Particle::mass() const {
  return _mass;
}

glm::vec3 Particle::position() const {
  return _position;
}
glm::vec3 Particle::velocity() const {
  return _velocity;
}
