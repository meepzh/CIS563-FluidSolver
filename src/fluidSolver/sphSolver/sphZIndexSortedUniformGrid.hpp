//
//  sphZIndexSortedUniformGrid.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_SPHZINDEXSORTEDUNIFORMGRID_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_SPHZINDEXSORTEDUNIFORMGRID_HPP_

#include "sphGrid.hpp"

#include "../zCurve.hpp"

struct ZISData {
  SPHParticle *p;
  unsigned int count;
};

// Grid type XZY (X elements together, then Z, then Y)
class SPHZIndexSortedUniformGrid : public SPHGrid {
public:
  SPHZIndexSortedUniformGrid(const glm::vec3 &minBounds, const glm::vec3 &maxBounds, float cellSize, std::vector<SPHParticle> *master);
  ~SPHZIndexSortedUniformGrid();

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
  std::vector<SPHParticle> *master;
  std::vector<ZISData> cells;
  ZCurve zCurve;
  unsigned long zMax;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_SPHZINDEXSORTEDUNIFORMGRID_HPP_ */