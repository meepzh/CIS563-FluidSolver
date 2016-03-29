//
//  sphUniformGrid.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_SPHUNIFORMGRID_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_SPHUNIFORMGRID_HPP_

#include "sphGrid.hpp"

// Grid type XZY (X elements together, then Z, then Y)
class SPHUniformGrid : public SPHGrid {
public:
  SPHUniformGrid(const glm::vec3 &minBounds, const glm::vec3 &maxBounds, float cellSize);
  ~SPHUniformGrid();

  virtual void addParticle(SPHParticle *p);
  virtual void updateParticle(SPHParticle *p);
  virtual void getNeighbors(SPHParticle *p);
  virtual void clear();
  virtual glm::ivec3 getGridCoordinates(const glm::vec3 &pt);
  virtual unsigned int getIndex(const glm::ivec3 &c);
  virtual unsigned int getIndex(unsigned int x, unsigned int y, unsigned int z);
  virtual void printDiagnostics();

  #if MFluidSolver_USE_OPENVDB
  virtual void exportVDB(std::string &file, std::string &gridName);
  #endif

private:
  std::vector<std::vector<SPHParticle *>> *data;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_SPHUNIFORMGRID_HPP_ */