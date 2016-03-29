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

struct InvalidSPHGridSize : std::exception {
  const char *what() const noexcept {return "Invalid SPH grid size.\n";};
};

// Grid type XZY (X elements together, then Z, then Y)
class SPHGrid {
public:
  SPHGrid(const glm::vec3 &minBounds, const glm::vec3 &maxBounds, float cellSize);
  ~SPHGrid();

  void addParticle(SPHParticle *p);
  void getNeighbors(SPHParticle *p);
  void clear();
  glm::ivec3 getGridCoordinates(const glm::vec3 &pt);
  unsigned int getIndex(const glm::ivec3 &c);
  unsigned int getIndex(unsigned int x, unsigned int y, unsigned int z);
  void printDiagnostics();

  #if MFluidSolver_USE_OPENVDB
  void exportVDB(std::string &file, std::string &gridName);
  #endif

private:
  std::vector<std::vector<SPHParticle *>> *data;
  glm::vec3 minBounds;
  glm::vec3 maxBounds;
  glm::ivec3 cellBounds;
  unsigned int numCells;
  float cellSize;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_SPHGRID_HPP_ */