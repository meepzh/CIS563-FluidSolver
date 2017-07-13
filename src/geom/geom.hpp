//  Copyright 2016 Robert Zhou
//
//  geom.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_GEOM_HPP_
#define MFLUIDSOLVER_GEOM_HPP_

class Geometry;
class FluidSolver;

#include <random>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "MFluidSolverConfig.hpp"

#include "fluidSolver/fluidSolver.hpp"
#include "drawable.hpp"
#include "transform.hpp"

enum ParticleSpawnMethod {Uniform, Jittered, PoissonDisk};

class Geometry : public Drawable {
 public:
  Geometry();
  void getBoundsByTransformedMinMax(
    const glm::vec3 &min, const glm::vec3 &max,
    glm::vec3 *outMin, glm::vec3 *outMax) const;
  virtual bool intersects(const glm::vec3 &position) const = 0;
  virtual bool intersects(const glm::vec3 &position,
                          glm::ivec3 *violations) const = 0;
  virtual void spawnParticlesInVolume(FluidSolver *solver,
    ParticleSpawnMethod spawnMethod) = 0;

  std::string name;
  Transform transform;

 protected:
  void initRNG();
  std::mt19937 *rng;
};

#endif  // MFLUIDSOLVER_GEOM_HPP_
