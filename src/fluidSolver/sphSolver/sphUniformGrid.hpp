//  Copyright 2016 Robert Zhou
//
//  sphUniformGrid.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_SPHUNIFORMGRID_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_SPHUNIFORMGRID_HPP_

#include <string>
#include <vector>

#include "sphGrid.hpp"

// Grid type XZY (X elements together, then Z, then Y)
class SPHUniformGrid : public SPHGrid {
 public:
  SPHUniformGrid(const glm::vec3 &minBounds, const glm::vec3 &maxBounds,
    float cellSize);
  ~SPHUniformGrid();

  virtual void getNeighbors(SPHParticle *p);
  virtual void addParticle(SPHParticle *p);
  virtual void updateParticle(SPHParticle *p);
  virtual void clear();
  virtual void printDiagnostics();

  #if MFluidSolver_USE_OPENVDB
  virtual void exportVDB(const std::string &file, const std::string &gridName);
  #endif

 private:
  std::vector<std::vector<SPHParticle *>> data;
};

#endif  // MFLUIDSOLVER_FLUIDSOLVER_SPHUNIFORMGRID_HPP_
