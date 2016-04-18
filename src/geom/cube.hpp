//
//  cube.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_GEOM_CUBE_HPP_
#define MFLUIDSOLVER_GEOM_CUBE_HPP_

#include "MFluidSolverConfig.hpp"

#include "geom.hpp"

class Cube : public Geometry {
public:
  Cube() : Cube(glm::vec3(MFluidSolver_DEFAULT_GEOMETRY_COLOR_R, MFluidSolver_DEFAULT_GEOMETRY_COLOR_G, MFluidSolver_DEFAULT_GEOMETRY_COLOR_B)) {}
  Cube(const glm::vec3 &color);

  virtual GLenum drawMode();
  virtual bool intersects(const glm::vec3 &point) const;
  virtual bool intersects(const glm::vec3 &point, glm::ivec3 &violations) const;
  virtual void spawnParticlesInVolume(FluidSolver *solver, ParticleSpawnMethod spawnMethod) const;

private:
  glm::vec3 _color;
};

#endif /* MFLUIDSOLVER_GEOM_CUBE_HPP_ */
