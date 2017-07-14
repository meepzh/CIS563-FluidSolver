//  Copyright 2016 Robert Zhou
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
  explicit SPHParticle(const glm::vec3 &position) : SPHParticle(1, position) {}
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
  inline float advectionDiagonal() const;
  inline glm::vec3 advectionDisplacementEstimate() const;
  inline float densityIntermediate() const;
  inline glm::vec3 sumPressureDisplacementFromNeighbors() const;
  inline glm::vec3 velocityIntermediate() const;

  inline void setAdvectionDiagonal(float a);
  inline void setAdvectionDisplacementEstimate(const glm::vec3 &d);
  inline void setDensityIntermediate(float d);
  inline void setSumPressureDisplacementFromNeighbors(const glm::vec3 &d);
  inline void setVelocityIntermediate(const glm::vec3 &v);

  // Index
  uint32_t index;
  static bool indexCompare(const SPHParticle &a, const SPHParticle &b);

 protected:
  float _density;
  float _pressure;
  glm::vec3 _nonPressureForce;
  glm::vec3 _pressureForce;
  glm::vec3 _oldPosition;
  std::vector<SPHParticle *> _neighbors;

  // IISPH
  float _advectionDiagonal;
  glm::vec3 _advectionDisplacementEstimate;
  float _densityIntermediate;
  glm::vec3 _sumPressureDisplacementFromNeighbors;
  glm::vec3 _velocityIntermediate;
};

#include "sphParticle.inline.hpp"

#endif  // MFLUIDSOLVER_FLUIDSOLVER_SPHSOLVER_SPHPARTICLE_HPP_
