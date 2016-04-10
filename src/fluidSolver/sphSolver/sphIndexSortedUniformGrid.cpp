//
//  sphIndexSortedUniformGrid.cpp
//  MFluidSolver

#include "sphIndexSortedUniformGrid.hpp"

#include <algorithm>

#if MFluidSolver_USE_TBB
#include <tbb/parallel_for.h>
#endif

#include "utils.hpp"

SPHIndexSortedUniformGrid::SPHIndexSortedUniformGrid(const glm::vec3 &minBounds, const glm::vec3 &maxBounds, float cellSize, std::vector<SPHParticle> *master)
 : SPHGrid(minBounds, maxBounds, cellSize), master(master){
  cells.resize(numCells);
}

SPHIndexSortedUniformGrid::~SPHIndexSortedUniformGrid() {

}

void SPHIndexSortedUniformGrid::getNeighbors(SPHParticle *p) {
  glm::ivec3 pC = getGridCoordinates(p->position());
  std::vector<SPHParticle *> *neighbors = p->neighbors();
  neighbors->clear();

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_TRACE
  std::cout << "TRACE: Getting neighbors for cell (" << pC.x << ", " << pC.y << ", " << pC.z << ")" << std::endl;
  #endif

  glm::ivec3 coords;
  for (int i = -1; i < 2; ++i) {
    for (int j = -1; j < 2; ++j) {
      for (int k = -1; k < 2; ++k) {
        coords = glm::ivec3(pC.x + i, pC.y + j, pC.z + k);
        // If not at original cell, add all neighbors (process later in kernel)
        if (coords.x >= 0 && coords.y >= 0 && coords.z >= 0 &&
            coords.x < cellBounds.x && coords.y < cellBounds.y && coords.z < cellBounds.z) {
          unsigned long index = getIndex(coords);

          #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_TRACE
          unsigned int numNeighbors = 0;
          std::cout << "TRACE: Accessing cell (" << coords.x << ", " << coords.y << ", " << coords.z << ")" << std::endl;
          #endif

          SPHParticle *c = cells.at(index);
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
            } while(index == c->index);
          }

          #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_TRACE
          std::cout << "TRACE: Found " << numNeighbors << " neighbor(s)" << std::endl;
          #endif
        }
      } // end for k
    } // end for j
  } // end for i
}

void SPHIndexSortedUniformGrid::addParticle(SPHParticle *p) {

}

void SPHIndexSortedUniformGrid::updateParticle(SPHParticle *p) {

}

void SPHIndexSortedUniformGrid::clear() {
  for (unsigned int i = 0; i < cells.size(); ++i) {
    cells.at(i) = nullptr;
  }
}

void SPHIndexSortedUniformGrid::printDiagnostics() {
  std::cout << "No diagnostics available for index sorted uniform grids." << std::endl;
}

void SPHIndexSortedUniformGrid::resetAndFillCells(bool initialSort) {
  clear();
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
  cells.at(master->at(0).index) = &(master->at(0));

  #if MFluidSolver_USE_TBB
  tbb::parallel_for(tbb::blocked_range<size_t>(1, master->size()),
    [&](const tbb::blocked_range<size_t> &r) {
      for (unsigned int i = r.begin(); i != r.end(); ++i) {
  #else
      for (unsigned int i = 1; i < master->size(); ++i) {
  #endif
        if (master->at(i).index != master->at(i - 1).index) {
          cells.at(master->at(i).index) = &(master->at(i));
        }
  #if MFluidSolver_USE_TBB
      }
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
    MUtils::insertionSort(master->begin(), master->end(), SPHParticle::indexCompare);
  }
  endParticle = &(master->at(master->size() - 1));
}

#if MFluidSolver_USE_OPENVDB
void SPHIndexSortedUniformGrid::exportVDB(std::string &file, std::string &gridName) {

}
#endif
