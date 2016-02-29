//
//  particle.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_

#include <glm/glm.hpp>

class Particle {
public:
  Particle() : Particle(1) {};
  Particle(float mass);

  void addForce(glm::vec3 force);
  void update();

  glm::vec3 force();
  float mass();
  glm::vec3 position();
  glm::vec3 velocity();

private:
  glm::vec3 currentForce;
  float _mass;
  glm::vec3 _position;
  glm::vec3 _velocity;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_ */