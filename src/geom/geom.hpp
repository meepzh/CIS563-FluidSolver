//
//  geom.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_GEOM_HPP_
#define MFLUIDSOLVER_GEOM_HPP_

class Geometry;
class FluidSolver;

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "../fluidSolver/fluidSolver.hpp"
#include "drawable.hpp"
#include "transform.hpp"

class Geometry : public Drawable {
public:
  void getBoundsByTransformedMinMax(const glm::vec3 &min, const glm::vec3 &max, glm::vec3 &outMin, glm::vec3 &outMax) const;
  virtual bool intersects(const glm::vec3 &position) const = 0;
  virtual bool intersects(const glm::vec3 &position, glm::ivec3 &violations) const = 0;
  virtual void spawnParticlesInVolume(FluidSolver *solver) const = 0;

  std::string name;
  Transform transform;
};

#endif /* MFLUIDSOLVER_GEOM_HPP_ */
