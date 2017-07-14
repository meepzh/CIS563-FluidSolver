//  Copyright 2016 Robert Zhou
//
//  fluidSolver.inline.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_FLUIDSOLVER_INLINE_HPP_
#define MFLUIDSOLVER_FLUIDSOLVER_INLINE_HPP_

#if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_DEBUG
#include <iostream>
#endif

// Simulation End
inline bool FluidSolver::checkIfEnded() {
  // Record times and updates
  if (firstRun) {
    startTime = tbb::tick_count::now();
    firstRun = false;
  }
  if (endedSimulation) {
    return true;
  } else if (limitNumUpdates && numUpdates >= maxUpdates) {
    endTime = tbb::tick_count::now();
    endedSimulation = true;
    return true;
  } else {
    ++numUpdates;
  }
  return false;
}

inline bool FluidSolver::hasEndedSimulation() {
  return endedSimulation;
}

inline bool FluidSolver::shouldPauseSimulation() {
  return _shouldPauseSimulation;
}

// Helpers
inline void FluidSolver::logTimestep() {
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_DEBUG
  std::cout << "DEBUG: Updating by " << _fixedTimestep << " seconds" << std::endl;
  #endif
}

#endif  // MFLUIDSOLVER_FLUIDSOLVER_INLINE_HPP_
