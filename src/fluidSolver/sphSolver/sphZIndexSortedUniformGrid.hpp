//  Copyright 2016 Robert Zhou
//
//  sphZIndexSortedUniformGrid.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_SPHZINDEXSORTEDUNIFORMGRID_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_SPHZINDEXSORTEDUNIFORMGRID_HPP_

#include <vector>

#include "sphIndexSortedUniformGrid.hpp"
#include "fluidSolver/zCurve.hpp"

// Grid type XZY (X elements together, then Z, then Y)
class SPHZIndexSortedUniformGrid : public SPHIndexSortedUniformGrid {
 public:
  SPHZIndexSortedUniformGrid(
    const glm::vec3 &minBounds, const glm::vec3 &maxBounds,
    float cellSize, std::vector<SPHParticle> *master);
  ~SPHZIndexSortedUniformGrid();

  virtual uint32_t getIndex(
    unsigned int x, unsigned int y, unsigned int z);

 private:
  ZCurve zCurve;
};

#endif  // MFLUIDSOLVER_FLUIDSOLVER_SPHZINDEXSORTEDUNIFORMGRID_HPP_
