//
//  geom.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_GEOM_HPP_
#define MFLUIDSOLVER_GEOM_HPP_

#include <glm/glm.hpp>
#include <string>
#include "drawable.hpp"
#include "transform.hpp"

class Geometry : public Drawable {
public:
  virtual bool intersects(const glm::vec3 &position) const = 0;

  std::string name;
  Transform transform;
};

#endif /* MFLUIDSOLVER_GEOM_HPP_ */
