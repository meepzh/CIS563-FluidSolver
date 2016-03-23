//
//  sphParticle.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_SPHSOLVER_SPHPARTICLE_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_SPHSOLVER_SPHPARTICLE_HPP_

#include <vector>
#include "../particle.hpp"

class SPHParticle : public Particle {
public:
  SPHParticle() : SPHParticle(1, glm::vec3(0)) {}
  SPHParticle(const glm::vec3 &position) : SPHParticle(1, position) {}
  SPHParticle(float mass, const glm::vec3 &position);

  // Computation
  virtual void update(double deltaT);

  // Properties
  float density() const;
  float pressure() const;
  glm::vec3 forceDensity() const;

  void setDensity(float density);
  void setPressure(float pressure);
  void setForceDensity(const glm::vec3 &forceDensity);

  std::vector<SPHParticle *> &neighbors();
  void clearNeighbors();

protected:
  float _density;
  float _pressure;
  glm::vec3 _forceDensity;
  std::vector<SPHParticle *> _neighbors;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_SPHSOLVER_SPHPARTICLE_HPP_ */