//
//  particle.cpp
//  MFluidSolver

#include "particle.hpp"

Particle::Particle(float mass) : color(0, 0, 1) {
  // State
  state.mass = 1;
  state.position = glm::vec3(0);
  state.velocity = glm::vec3(0);
  state.acceleration = glm::vec3(0);
}

void Particle::addForce(glm::vec3 force) {
  state.acceleration += force / state.mass;
}

void Particle::update(double deltaT) {
  state.velocity += state.acceleration * (float)deltaT;
  state.position += state.velocity * (float)deltaT;

  // Reset forces
  state.acceleration = glm::vec3(0);
}

float Particle::mass() const {
  return state.mass;
}
glm::vec3 Particle::position() const {
  return state.position;
}
glm::vec3 Particle::velocity() const {
  return state.velocity;
}
glm::vec3 Particle::acceleration() const {
  return state.acceleration;
}
glm::vec3 Particle::force() const {
  return state.acceleration * state.mass;
}