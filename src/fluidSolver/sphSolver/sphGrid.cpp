//
//  sphGrid.cpp
//  MFluidSolver

#include "sphGrid.hpp"

#include <iostream>

SPHGrid::SPHGrid(const glm::vec3 &minBounds, const glm::vec3 &maxBounds, float cellSize)
 : minBounds(minBounds), maxBounds(maxBounds), cellSize(cellSize) {
  glm::vec3 gridSize = maxBounds - minBounds;
  if (gridSize.x < 0 || gridSize.y < 0 || gridSize.z < 0) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_FATAL
    std::cerr << "FATAL: Grid size (" << gridSize.x << ", " << gridSize.y << ", " << gridSize.z << ") is invalid" << std::endl;
    #endif
    throw SPHGridNegativeSizeException();
  }

  cellBounds = (gridSize + 0.500001f) / cellSize;
  numCells = cellBounds.x * cellBounds.y * cellBounds.z;
}
