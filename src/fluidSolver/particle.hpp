//
//  particle.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_

#include "MFluidSolverConfig.hpp"

#include <glm/glm.hpp>

class Particle {
public:
  // Constructors / Destructors
  Particle() : Particle(1, glm::vec3(0)) {}
  Particle(const glm::vec3 &position) : Particle(1, position) {}
  Particle(float mass, const glm::vec3 &position);

  // Operators
  virtual bool operator==(const Particle &b) const;

  // Update
  inline void setPosition(const glm::vec3 &position);
  inline void reverseVelocity(const glm::ivec3 &directions, float bounceCoefficient = MFluidSolver_DEFAULT_PARTICLE_BOUNCE);
  inline void stopVelocity(const glm::ivec3 &directions);

  // Properties
  glm::vec3 color;
  inline float mass() const;
  inline glm::vec3 position() const;
  inline glm::vec3 velocity() const;

protected:
  float _mass;
  glm::vec3 _position;
  glm::vec3 _velocity;
};

#include "fluidSolver/particle.inline.hpp"

#endif /* MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_ */