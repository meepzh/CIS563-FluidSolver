//
//  cube.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_GEOM_CUBE_HPP_
#define MFLUIDSOLVER_GEOM_CUBE_HPP_

#include "geom.hpp"

class Cube : public Geometry {
public:
  Cube() : Cube(glm::vec3(1.f)) {}
  Cube(const glm::vec3 &color);

  virtual GLenum drawMode();
  virtual bool intersects(const glm::vec3 &point) const;

private:
  glm::vec3 _color;
};

#endif /* MFLUIDSOLVER_GEOM_CUBE_HPP_ */
