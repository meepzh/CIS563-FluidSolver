//
//  particle.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_

#include <glm/glm.hpp>

struct State {
  float mass;
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 acceleration;
};

class Particle {
public:
  // Constructors/Destructors
  Particle() : Particle(1) {};
  Particle(float mass);

  // Force
  void addForce(glm::vec3 force);
  void update();

  // Properties
  glm::vec3 color;
  float mass() const;
  glm::vec3 position() const;
  glm::vec3 velocity() const;
  glm::vec3 acceleration() const;
  glm::vec3 force() const;

private:
  State state;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_ */