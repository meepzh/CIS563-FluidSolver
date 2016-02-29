//
//  particle.cpp
//  MFluidSolver

#include "particle.hpp"

Particle::Particle(float mass) : _mass(mass), currentForce(0) {}

void Particle::addForce(glm::vec3 force) {
  currentForce += force;
}

void Particle::update() {
  currentForce /= _mass;

  _velocity += currentForce;
  _position += _velocity;

  currentForce = glm::vec3(0);
}

glm::vec3 Particle::force() {
  return currentForce;
}
float Particle::mass() {
  return _mass;
}
glm::vec3 Particle::position() {
  return _position;
}
glm::vec3 Particle::velocity() {
  return _velocity;
}