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

void Particle::update() {
  state.velocity += state.acceleration;
  state.position += state.velocity;

  // Reset forces
  state.acceleration = glm::vec3(0);
}

float Particle::mass() {
  return state.mass;
}
glm::vec3 Particle::position() {
  return state.position;
}
glm::vec3 Particle::velocity() {
  return state.velocity;
}
glm::vec3 Particle::acceleration() {
  return state.acceleration;
}
glm::vec3 Particle::force() {
  return state.acceleration * state.mass;
}