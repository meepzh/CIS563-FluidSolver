//
//  particle.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_

#include <glm/glm.hpp>

class Particle {
public:
  // Constructors/Destructors
  Particle() : Particle(1, glm::vec3(0)) {}
  Particle(const glm::vec3 &position) : Particle(1, position) {}
  Particle(float mass, const glm::vec3 &position);

  // Update
  virtual void update(double deltaT);
  void reverseVelocity(const glm::ivec3 &directions);
  void stopVelocity(const glm::ivec3 &directions);
  void setPosition(const glm::vec3 &position);
  virtual void undoUpdate(double deltaT);

  // Properties
  glm::vec3 color;
  float mass() const;
  glm::vec3 position() const;
  glm::vec3 velocity() const;

protected:
  float _mass;
  glm::vec3 _position;
  glm::vec3 _velocity;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_ */