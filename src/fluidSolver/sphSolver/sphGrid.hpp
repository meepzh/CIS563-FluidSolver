//
//  sphGrid.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_SPHGRID_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_SPHGRID_HPP_

#include "MFluidSolverConfig.hpp"

#include <exception>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "sphParticle.hpp"

struct InvalidSPHGridSizeException : std::exception {
  const char *what() const noexcept {return "Invalid SPH grid size.\n";};
};

class SPHGrid {
public:
  SPHGrid(const glm::vec3 &minBounds, const glm::vec3 &maxBounds, float cellSize);

  virtual void addParticle(SPHParticle *p) = 0;
  virtual void updateParticle(SPHParticle *p) = 0;
  virtual void getNeighbors(SPHParticle *p) = 0;
  virtual void clear() = 0;
  virtual glm::ivec3 getGridCoordinates(const glm::vec3 &pt) = 0;
  virtual unsigned int getIndex(const glm::ivec3 &c) = 0;
  virtual unsigned int getIndex(unsigned int x, unsigned int y, unsigned int z) = 0;
  virtual void printDiagnostics() = 0;

  #if MFluidSolver_USE_OPENVDB
  virtual void exportVDB(std::string &file, std::string &gridName) = 0;
  #endif

protected:
  glm::vec3 minBounds;
  glm::vec3 maxBounds;
  glm::ivec3 cellBounds;
  unsigned int numCells;
  float cellSize;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_SPHGRID_HPP_ */