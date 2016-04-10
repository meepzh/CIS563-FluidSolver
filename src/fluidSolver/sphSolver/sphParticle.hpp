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
  inline glm::vec3 force() const;
  inline glm::vec3 nonPressureForce() const;
  inline glm::vec3 oldPosition() const;
  inline glm::vec3 pressureForce() const;

  inline void setDensity(float density);
  inline void setNonPressureForce(const glm::vec3 &force);
  inline void setPressure(float pressure);
  inline void setPressureForce(const glm::vec3 &force);
  inline void setVelocity(const glm::vec3 &velocity);

  inline std::vector<SPHParticle *> *neighbors();
  inline void clearNeighbors();

  // IISPH Properties
  inline float aSelf() const;
  inline glm::vec3 dSelf() const;
  inline glm::vec3 dNeighbors() const;
  inline float densityIntermediate() const;

  inline void setASelf(float a);
  inline void setDSelf(const glm::vec3 &d);
  inline void setDNeighbors(const glm::vec3 &d);
  inline void setDensityIntermediate(float d);

  // Index
  unsigned long index;
  static bool indexCompare(const SPHParticle &a, const SPHParticle &b);

protected:
  float _density;
  float _pressure;
  glm::vec3 _nonPressureForce;
  glm::vec3 _pressureForce;
  glm::vec3 _oldPosition;
  std::vector<SPHParticle *> _neighbors;

  // IISPH
  float _aSelf;
  glm::vec3 _dSelf;
  glm::vec3 _dNeighbors;
  float _densityIntermediate;
};

#include "sphParticle.inline.hpp"

#endif /* MFLUIDSOLVER_FLUIDSOLVER_SPHSOLVER_SPHPARTICLE_HPP_ */