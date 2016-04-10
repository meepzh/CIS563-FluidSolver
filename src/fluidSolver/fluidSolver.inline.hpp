//
//  fluidSolver.inline.hpp
//  MFluidSolver

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

// Helpers
inline void FluidSolver::logTimestep() {
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_DEBUG
  std::cout << "DEBUG: Updating by " << deltaT << " seconds" << std::endl;
  #endif
}