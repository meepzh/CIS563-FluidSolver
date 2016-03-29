//
//  sphUniformGrid.cpp
//  MFluidSolver

#include "sphUniformGrid.hpp"

#include <cassert>

#if MFluidSolver_USE_OPENVDB
#include <openvdb/openvdb.h>
#endif

SPHUniformGrid::SPHUniformGrid(const glm::vec3 &minBounds, const glm::vec3 &maxBounds, float cellSize)
 : SPHGrid(minBounds, maxBounds, cellSize) {
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std:: cout << "INFO: Created grid with cell bounds: (" << cellBounds.x << ", " << cellBounds.y << ", " << cellBounds.z << ")" << std::endl;
  #endif

  data = new std::vector<std::vector<SPHParticle *>>();
  data->resize(numCells);
}

SPHUniformGrid::~SPHUniformGrid() {
  delete data;
}

void SPHUniformGrid::addParticle(SPHParticle *p) {
  glm::ivec3 pC = getGridCoordinates(p->position());
  unsigned int pI = getIndex(pC);
  data->at(pI).push_back(p);
}

void SPHUniformGrid::updateParticle(SPHParticle *p) {
  glm::ivec3 oldCoords = getGridCoordinates(p->oldPosition());
  glm::ivec3 newCoords = getGridCoordinates(p->position());
  if (oldCoords != newCoords) {
    unsigned int oldIndex = getIndex(oldCoords);
    unsigned int newIndex = getIndex(newCoords);

    std::vector<SPHParticle *> *oldCell = &(data->at(oldIndex));
    auto oldIt = std::find(std::begin(*oldCell), std::end(*oldCell), p);
    if (oldIt != std::end(*oldCell)) {
      oldCell->erase(oldIt);
      data->at(newIndex).push_back(p);
    } else {
      #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_ERROR
      std::cerr << "ERROR: Could not find particle at (" <<
        p->position().x << ", " << p->position().y << ", " << p->position().z <<
        ") based on old position (" <<
        p->oldPosition().x << ", " << p->oldPosition().y << ", " << p->oldPosition().z <<
        "), index " << oldIndex << std::endl;
      #endif
    }
  }
}

void SPHUniformGrid::getNeighbors(SPHParticle *p) {
  glm::ivec3 pC = getGridCoordinates(p->position());
  std::vector<SPHParticle *> *neighbors = p->neighbors();

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_TRACE
  std::cout << "TRACE: Getting neighbors for cell (" << pC.x << ", " << pC.y << ", " << pC.z << ")" << std::endl;
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
          std::cout << "TRACE: Accessing cell (" << coords.x << ", " << coords.y << ", " << coords.z << ")" << std::endl;
          #endif
        }
      } // end for k
    } // end for j
  } // end for i
}

void SPHUniformGrid::clear() {
  for (unsigned int i = 0; i < numCells; ++i) {
    data->at(i).clear();
  }
}

glm::ivec3 SPHUniformGrid::getGridCoordinates(const glm::vec3 &pt) {
  return glm::ivec3((int)((pt.x - minBounds.x) / cellSize), (int)((pt.y - minBounds.y) / cellSize), (int)((pt.z - minBounds.z) / cellSize));
}

unsigned int SPHUniformGrid::getIndex(unsigned int x, unsigned int y, unsigned int z) {
  return y * cellBounds.z * cellBounds.x + z * cellBounds.x + x;
}

unsigned int SPHUniformGrid::getIndex(const glm::ivec3 &c) {
  #if MFluidSolver_USE_ASSERTS
  assert(c.x >= 0 && c.y >= 0 && c.z >= 0);
  assert(c.x < cellBounds.x && c.y < cellBounds.y && c.z < cellBounds.z);
  #endif

  return getIndex(c.x, c.y, c.z);
}

void SPHUniformGrid::printDiagnostics() {
  std::cout << "Grid bounds: (" << cellBounds.x << ", " << cellBounds.y << ", " << cellBounds.z << ")" << std::endl;
  std::cout << "Grid size: " << numCells << std::endl;

  for (unsigned int i = 0; i < cellBounds.x; ++i) {
    for (unsigned int j = 0; j < cellBounds.y; ++j) {
      for (unsigned int k = 0; k < cellBounds.z; ++k) {
        std::printf("(%d, %d, %d) - %d\n", i, j, k, data->at(getIndex(i, j, k)).size());
      }
    }
  }
}

#if MFluidSolver_USE_OPENVDB
void SPHUniformGrid::exportVDB(std::string &file, std::string &gridName) {
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
  std::cout << "INFO: Exported grid VDB to: " << file << std::endl;
  #endif
}
#endif