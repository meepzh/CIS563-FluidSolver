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

float Particle::mass() const {
  return _mass;
}

glm::vec3 Particle::position() const {
  return _position;
}
glm::vec3 Particle::velocity() const {
  return _velocity;
}
