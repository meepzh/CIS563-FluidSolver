//
//  sphParticle.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_SPHSOLVER_SPHPARTICLE_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_SPHSOLVER_SPHPARTICLE_HPP_

#include <vector>

#include "fluidSolver/particle.hpp"

class SPHParticle : public Particle {
public:
  SPHParticle() : SPHParticle(1, glm::vec3(0)) {}
  SPHParticle(const glm::vec3 &position) : SPHParticle(1, position) {}
  SPHParticle(float mass, const glm::vec3 &position);

  virtual inline void update(const glm::vec3 &newVel, const glm::vec3 &newPos);

  // Properties
  inline float density() const;
  inline float pressure() const;
  inline glm::vec3 forceDensity() const;
  inline glm::vec3 nonPressureForceDensity() const;
  inline glm::vec3 oldPosition() const;
  inline glm::vec3 pressureForceDensity() const;

  inline void setDensity(float density);
  inline void setNonPressureForceDensity(const glm::vec3 &forceDensity);
  inline void setPressure(float pressure);
  inline void setPressureForceDensity(const glm::vec3 &forceDensity);

  inline std::vector<SPHParticle *> *neighbors();
  inline void clearNeighbors();

  unsigned long index;
  static bool indexCompare(const SPHParticle &a, const SPHParticle &b);

protected:
  float _density;
  float _pressure;
  glm::vec3 _nonPressureForceDensity;
  glm::vec3 _pressureForceDensity;
  glm::vec3 _oldPosition;
  std::vector<SPHParticle *> _neighbors;
};

#include "sphParticle.inline.hpp"

#endif /* MFLUIDSOLVER_FLUIDSOLVER_SPHSOLVER_SPHPARTICLE_HPP_ */