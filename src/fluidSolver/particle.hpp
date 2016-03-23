//
//  particle.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_

#include <glm/glm.hpp>

class Particle {
public:
  // Constructors/Destructors
  Particle() : Particle(glm::vec3(0)) {}
  Particle(const glm::vec3 &position);

  // Update
  virtual void update(double deltaT);

  // Properties
  glm::vec3 color;
  glm::vec3 position() const;
  glm::vec3 velocity() const;

protected:
  glm::vec3 _position;
  glm::vec3 _velocity;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_PARTICLE_HPP_ */