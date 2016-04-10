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

  virtual void update(const glm::vec3 &newVel, const glm::vec3 &newPos);

  // Properties
  float density() const;
  float pressure() const;
  glm::vec3 forceDensity() const;
  glm::vec3 oldPosition() const;

  void setDensity(float density);
  void setPressure(float pressure);
  void setForceDensity(const glm::vec3 &forceDensity);
  void addForceDensity(const glm::vec3 &forceDensity);

  std::vector<SPHParticle *> *neighbors();
  void clearNeighbors();

  unsigned long index;
  static bool indexCompare(const SPHParticle &a, const SPHParticle &b);

protected:
  float _density;
  float _pressure;
  glm::vec3 _forceDensity;
  glm::vec3 _oldPosition;
  std::vector<SPHParticle *> _neighbors;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_SPHSOLVER_SPHPARTICLE_HPP_ */