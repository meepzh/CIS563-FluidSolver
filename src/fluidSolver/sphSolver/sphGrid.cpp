//
//  sphGrid.cpp
//  MFluidSolver

#include "sphGrid.hpp"

#include <cassert>
#include <cstdio>

#if MFluidSolver_USE_OPENVDB
#include <openvdb/openvdb.h>
#endif

SPHGrid::SPHGrid(const glm::vec3 &minBounds, const glm::vec3 &maxBounds, float cellSize)
 : minBounds(minBounds), maxBounds(maxBounds), cellSize(cellSize) {
  glm::vec3 gridSize = maxBounds - minBounds;
  if (gridSize.x <= 0 || gridSize.y <= 0 || gridSize.z <= 0) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_FATAL
    std::fprintf(stderr, "FATAL: Grid size (%.2f, %.2f, %.2f) is invalid\n", gridSize.x, gridSize.y, gridSize.z);
    #endif
    throw InvalidSPHGridSize();
  }

  cellBounds = (gridSize + 0.5f) / cellSize;

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::printf("INFO: Created grid with cell bounds: (%d, %d, %d)\n", cellBounds.x, cellBounds.y, cellBounds.z);
  #endif

  data = new std::vector<std::vector<SPHParticle *>>();
  numCells = cellBounds.x * cellBounds.y * cellBounds.z;
  data->resize(numCells);
}

SPHGrid::~SPHGrid() {
  delete data;
}

void SPHGrid::addParticle(SPHParticle *p) {
  glm::ivec3 pC = getGridCoordinates(p->position());
  unsigned int pI = getIndex(pC);
  data->at(pI).push_back(p);
}

void SPHGrid::getNeighbors(SPHParticle *p) {
  glm::ivec3 pC = getGridCoordinates(p->position());
  std::vector<SPHParticle *> *neighbors = p->neighbors();

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_TRACE
  std::printf("TRACE: Getting neighbors for cell (%d, %d, %d)\n", pC.x, pC.y, pC.z);
  #endif

  unsigned int index;
  std::vector<SPHParticle *> *vec;

  glm::ivec3 coords;
  for (int i = -1; i < 2; ++i) {
    for (int j = -1; j < 2; ++j) {
      for (int k = -1; k < 2; ++k) {
        coords = glm::ivec3(pC.x + i, pC.y + j, pC.z + k);
        // If not at original cell, add all neighbors (process later in kernel)
        if (coords.x >= 0 && coords.y >= 0 && coords.z >= 0 &&
            coords.x < cellBounds.x && coords.y < cellBounds.y && coords.z < cellBounds.z) {
          index = getIndex(coords);
          vec = &(data->at(index));
          for (unsigned int l = 0; l < vec->size(); ++l) {
            neighbors->push_back(vec->at(l));
          } // end for l

          #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_TRACE
          std::printf("TRACE: Accessing cell (%d, %d, %d)\n", coords.x, coords.y, coords.z);
          #endif
        }
      } // end for k
    } // end for j
  } // end for i
}

void SPHGrid::clear() {
  for (unsigned int i = 0; i < numCells; ++i) {
    data->at(i).clear();
  }
}

glm::ivec3 SPHGrid::getGridCoordinates(const glm::vec3 &pt) {
  return glm::ivec3((int)((pt.x - minBounds.x) / cellSize), (int)((pt.y - minBounds.y) / cellSize), (int)((pt.z - minBounds.z) / cellSize));
}

unsigned int SPHGrid::getIndex(unsigned int x, unsigned int y, unsigned int z) {
  return y * cellBounds.z * cellBounds.x + z * cellBounds.x + x;
}

unsigned int SPHGrid::getIndex(const glm::ivec3 &c) {
  #if MFluidSolver_USE_ASSERTS
  assert(c.x >= 0 && c.y >= 0 && c.z >= 0);
  assert(c.x < cellBounds.x && c.y < cellBounds.y && c.z < cellBounds.z);
  #endif

  return getIndex(c.x, c.y, c.z);
}

void SPHGrid::printDiagnostics() {
  std::printf("Grid bounds: (%d, %d, %d)\n", cellBounds.x, cellBounds.y, cellBounds.z);
  std::printf("Grid size: %d\n", numCells);

  for (unsigned int i = 0; i < cellBounds.x; ++i) {
    for (unsigned int j = 0; j < cellBounds.y; ++j) {
      for (unsigned int k = 0; k < cellBounds.z; ++k) {
        std::printf("(%d, %d, %d) - %d\n", i, j, k, data->at(getIndex(i, j, k)).size());
      }
    }
  }
}

#if MFluidSolver_USE_OPENVDB
void SPHGrid::exportVDB(std::string &file, std::string &gridName) {
  // http://www.openvdb.org/documentation/doxygen/codeExamples.html
  openvdb::FloatGrid::Ptr grid = openvdb::FloatGrid::create(/*background value=*/0);
  openvdb::FloatGrid::Accessor accessor = grid->getAccessor();
  openvdb::Coord coord;

  for (unsigned int i = 0; i < cellBounds.x; ++i) {
    for (unsigned int j = 0; j < cellBounds.y; ++j) {
      for (unsigned int k = 0; k < cellBounds.z; ++k) {
        unsigned int numParticles = data->at(getIndex(i, j, k)).size();
        if (numParticles > 0) {
          coord.reset(i, j, k);
          accessor.setValue(coord, numParticles);
        }
      }
    }
  }

  grid->setName(gridName);
  openvdb::io::File fileIO(file);

  // Add the grid pointer to a container.
  openvdb::GridPtrVec grids;
  grids.push_back(grid);

  fileIO.write(grids);
  fileIO.close();

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::printf("INFO: Exported grid VDB to: %s\n", file.c_str());
  #endif
}
#endif