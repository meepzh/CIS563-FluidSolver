//  Copyright 2016 Robert Zhou
//
//  sphIndexSortedUniformGrid.cpp
//  MFluidSolver

#include "sphIndexSortedUniformGrid.hpp"

#include <algorithm>
#include <string>
#include <vector>

#if MFluidSolver_USE_ASSERTS
#include <cassert>
#endif
#if MFluidSolver_USE_TBB
#include <tbb/parallel_for.h>
#endif

#include "utils.hpp"

SPHIndexSortedUniformGrid::SPHIndexSortedUniformGrid(
  const glm::vec3 &minBounds, const glm::vec3 &maxBounds,
  float cellSize, std::vector<SPHParticle> *master)
    : SPHGrid(minBounds, maxBounds, cellSize), master(master) {
  cells.resize(numCells);
}

SPHIndexSortedUniformGrid::~SPHIndexSortedUniformGrid() {
}

void SPHIndexSortedUniformGrid::getNeighbors(SPHParticle *p) {
  glm::ivec3 pC = getGridCoordinates(p->position());
  std::vector<SPHParticle *> *neighbors = p->neighbors();
  neighbors->clear();

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_TRACE
  std::cout << "TRACE: Getting neighbors for cell (" <<
                pC.x << ", " << pC.y << ", " << pC.z << ")" << std::endl;
  #endif

  // Search neighboring grid cells
  glm::ivec3 coords;
  for (int i = -1; i < 2; ++i) {
    for (int j = -1; j < 2; ++j) {
      for (int k = -1; k < 2; ++k) {
        coords = glm::ivec3(pC.x + i, pC.y + j, pC.z + k);
        // Note: we check kernel radius and self in neighbor search
        if (coords.x >= 0 && coords.y >= 0 && coords.z >= 0 &&
            coords.x < cellBounds.x &&
            coords.y < cellBounds.y &&
            coords.z < cellBounds.z) {
          uint32_t index = getIndex(coords);

          #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_TRACE
          unsigned int numNeighbors = 0;
          std::cout << "TRACE: Accessing cell (" <<
            coords.x << ", " << coords.y << ", " << coords.z << ")" <<
            std::endl;
          #endif

          // Add all particles in index sorted list
          SPHParticle *c = cells[index];
          if (c != nullptr) {
            do {
              neighbors->push_back(c);

              #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_TRACE
              ++numNeighbors;
              #endif

              if (c == endParticle) {
                #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_TRACE
                std::cout << "TRACE: Reached end particle" << std::endl;
                #endif
                break;
              }
              ++c;
            } while (index == c->index);
          }

          #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_TRACE
          std::cout << "TRACE: Found " <<
            numNeighbors << " neighbor(s)" << std::endl;
          #endif
        }
      }  // end for k
    }  // end for j
  }  // end for i
}

void SPHIndexSortedUniformGrid::addParticle(SPHParticle *p) {
}

void SPHIndexSortedUniformGrid::updateParticle(SPHParticle *p) {
}

void SPHIndexSortedUniformGrid::clear() {
  for (unsigned int i = 0; i < cells.size(); ++i) {
    cells[i] = nullptr;
  }
}

void SPHIndexSortedUniformGrid::printDiagnostics() {
  std::cout << "No diagnostics available for index sorted uniform grids." <<
    std::endl;
}

void SPHIndexSortedUniformGrid::resetAndFillCells(bool initialSort) {
  clear();
  if (master->empty()) return;
  updateParticleIndices();
  sortParticles(initialSort);
  insertSortedParticleListToGrid();
}

void SPHIndexSortedUniformGrid::updateParticleIndices() {
  for (SPHParticle &p : *master) {
    p.index = getIndex(p.position());
  }
}

void SPHIndexSortedUniformGrid::insertSortedParticleListToGrid() {
  #if MFluidSolver_USE_ASSERTS
  assert((*master)[0].index >= 0 && (*master)[0].index < cells.size());
  #endif
  cells[(*master)[0].index] = &((*master)[0]);

  // Set pointer in each cell to first particle with associated cell index
  #if MFluidSolver_USE_TBB
  tbb::parallel_for((size_t)1, master->size(),
    [&](size_t i) {
  #else
    for (size_t i = 1; i < master->size(); ++i) {
  #endif
      #if MFluidSolver_USE_ASSERTS
      assert((*master)[i - 1].index >= 0 &&
             (*master)[i - 1].index < cells.size());
      assert((*master)[i].index >= 0 &&
             (*master)[i].index < cells.size());
      #endif
      if ((*master)[i].index != (*master)[i - 1].index) {
        cells[(*master)[i].index] = &((*master)[i]);
      }
  #if MFluidSolver_USE_TBB
    }
  );
  #else
  }
  #endif
}

void SPHIndexSortedUniformGrid::sortParticles(bool initialSort) {
  if (initialSort) {
    std::sort(master->begin(), master->end(), SPHParticle::indexCompare);
  } else {
    // Insertion sort is faster for minimal changes in cell indices
    MUtils::insertionSort(master->begin(), master->end(),
      SPHParticle::indexCompare);
  }
  endParticle = &(master->back());
}

#if MFluidSolver_USE_OPENVDB
void SPHIndexSortedUniformGrid::exportVDB(
    const std::string &file, const std::string &gridName) {
  // TODO: export VDB
}
#endif
