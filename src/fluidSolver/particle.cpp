//
//  particle.cpp
//  MFluidSolver

#include "particle.hpp"

Particle::Particle(float mass, const glm::vec3 &position)
 : color(MFluidSolver_DEFAULT_PARTICLE_COLOR_R, MFluidSolver_DEFAULT_PARTICLE_COLOR_G, MFluidSolver_DEFAULT_PARTICLE_COLOR_B),
   _mass(mass), _position(position), _velocity(glm::vec3(0)) {
}

void Particle::update(double deltaT) {
  _position += _velocity * (float)deltaT;
}

void Particle::undoUpdate(double deltaT) {
  _position -= _velocity * (float)deltaT;
}

void Particle::reverseVelocity(const glm::ivec3 &directions, float bounceCoefficient) {
  if (directions.x != 0) _velocity.x *= -1 * bounceCoefficient;
  if (directions.y != 0) _velocity.y *= -1 * bounceCoefficient;
  if (directions.z != 0) _velocity.z *= -1 * bounceCoefficient;
}

void Particle::stopVelocity(const glm::ivec3 &directions) {
  if (directions.x != 0) _velocity.x = 0;
  if (directions.y != 0) _velocity.y = 0;
  if (directions.z != 0) _velocity.z = 0;
}

void Particle::setPosition(const glm::vec3 &position) {
  _position = position;
}

bool Particle::operator==(const Particle &b) const {
  return b.mass() == _mass && b.position() == _position && b.velocity() == _velocity;
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
