//
//  sphParticle.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_SPHSOLVER_SPHPARTICLE_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_SPHSOLVER_SPHPARTICLE_HPP_

#include <vector>
#include "../particle.hpp"

class SPHParticle : public Particle {
public:
  SPHParticle() : SPHParticle(glm::vec3(0)) {}
  SPHParticle(const glm::vec3 &position);

  // Properties
  float density() const;
  float pressure() const;
  glm::vec3 forceDensity() const;
  std::vector<Particle *> &neighbors();

protected:
  float _density;
  float _pressure;
  glm::vec3 _forceDensity;
  std::vector<Particle *> _neighbors;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_SPHSOLVER_SPHPARTICLE_HPP_ */