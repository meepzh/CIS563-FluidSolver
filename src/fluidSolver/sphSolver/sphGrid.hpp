//
//  sphGrid.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_SPHGRID_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_SPHGRID_HPP_

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "sphParticle.hpp"

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
  void exportVDB(std::string &file, std::string &gridName);

private:
  std::vector<std::vector<SPHParticle *>> *data;
  glm::vec3 minBounds;
  glm::vec3 maxBounds;
  glm::ivec3 cellBounds;
  unsigned int numCells;
  float cellSize;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_SPHGRID_HPP_ */