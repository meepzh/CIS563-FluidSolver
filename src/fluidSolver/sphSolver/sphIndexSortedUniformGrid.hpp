//
//  sphIndexSortedUniformGrid.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_SPHINDEXSORTEDUNIFORMGRID_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_SPHINDEXSORTEDUNIFORMGRID_HPP_

#include "sphGrid.hpp"

// Grid type XZY (X elements together, then Z, then Y)
class SPHIndexSortedUniformGrid : public SPHGrid {
public:
  SPHIndexSortedUniformGrid(const glm::vec3 &minBounds, const glm::vec3 &maxBounds, float cellSize, std::vector<SPHParticle> *master);
  ~SPHIndexSortedUniformGrid();

  // SPHGrid methods
  virtual void getNeighbors(SPHParticle *p);

  virtual void addParticle(SPHParticle *p);
  virtual void updateParticle(SPHParticle *p);
  virtual void clear();
  virtual void printDiagnostics();

  virtual void resetAndFillCells();
  virtual void updateParticleIndices();
  virtual void insertSortedParticleListToGrid();
  void sortParticles();

  #if MFluidSolver_USE_OPENVDB
  virtual void exportVDB(std::string &file, std::string &gridName);
  #endif

private:
  std::vector<SPHParticle> *master;
  std::vector<SPHParticle *> cells;
  SPHParticle *endParticle;
};

#endif /* MFLUIDSOLVER_FLUIDSOLVER_SPHINDEXSORTEDUNIFORMGRID_HPP_ */