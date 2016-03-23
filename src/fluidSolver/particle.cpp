//
//  particle.cpp
//  MFluidSolver

#include "particle.hpp"

Particle::Particle(const glm::vec3 &position)
 : color(0, 0, 1), _position(position), _velocity(glm::vec3(0)) {
}

void Particle::update(double deltaT) {
  _position += _velocity * (float)deltaT;
}

glm::vec3 Particle::position() const {
  return _position;
}
glm::vec3 Particle::velocity() const {
  return _velocity;
}
