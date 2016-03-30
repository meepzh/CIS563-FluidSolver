//
//  sphSolver.cpp
//  MFluidSolver

#include "sphSolver.hpp"

#include <fstream>
#include <iostream>
#include <json/json.h>

#if MFluidSolver_RECORD_PERFORMANCE
#include <ctime>
#endif

#include "../../utils.hpp"

SPHSolver::SPHSolver()
 : nSearch(nullptr), inited(false) {
  setDefaultConfig();
}

SPHSolver::~SPHSolver() {
  delete nSearch;
}

void SPHSolver::init(const glm::vec3 &gridMin, const glm::vec3 &gridMax) {
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: Initializing SPH Solver" << std::endl;
  #endif

  kernelFunctions.setKernelRadius(kernelRadius);

  switch (nSearchType) {
    case NeighborSearchType::Naive:
      #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
      std::cout << "INFO: SPH Solver using naive neighbor search" << std::endl;
      #endif

      nSearch = new NaiveNeighborSearch();
      break;
    case NeighborSearchType::IndexSortedUniformGrid:
      #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
      std::cout << "INFO: SPH Solver using index sorted uniform grid neighbor search" << std::endl;
      #endif

      nSearch = new IndexSortedUniformGridNeighborSearch(kernelRadius, gridMin, gridMax, kernelRadius, &_particles, false);
      break;
    case NeighborSearchType::ZIndexSortedUniformGrid:
      #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
      std::cout << "INFO: SPH Solver using Z-curve index sorted uniform grid neighbor search" << std::endl;
      #endif

      nSearch = new IndexSortedUniformGridNeighborSearch(kernelRadius, gridMin, gridMax, kernelRadius, &_particles, true);
      break;
    case NeighborSearchType::UniformGrid:
    default:
      #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
      std::cout << "INFO: SPH Solver using uniform grid neighbor search" << std::endl;
      #endif

      // Note: Assumes grid cell size is equal to kernelRadius
      nSearch = new UniformGridNeighborSearch(kernelRadius, gridMin, gridMax, kernelRadius);
      break;
  }

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  switch (visualizationType) {
    case FluidVisualizationType::Neighbors:
      std::cout << "INFO: Now visualizing neighbors" << std::endl;
      break;
    case FluidVisualizationType::None:
      std::cout << "INFO: Visualization disabled" << std::endl;
      break;
    case FluidVisualizationType::Velocity:
      std::cout << "INFO: Now visualizing normalized velocity" << std::endl;
      break;
  }

  if (muViscosity <= 0) {
    std::cout << "INFO: As per config, viscosity is disabled" << std::endl;
  }
  #endif

  inited = true;
}

void SPHSolver::setDefaultConfig() {
  if (checkInited()) return;
  kStiffness = SPHConfig_Default_kStiffness;
  muViscosity = SPHConfig_Default_muViscosity;
  mMass = SPHConfig_Default_mMass;
  dRestDensity = SPHConfig_Default_dRestDensity;
  dtTimestep = SPHConfig_Default_dtTimestep;
  kernelRadius = SPHConfig_Default_kernelRadius;
  nSearchType = SPHConfig_Default_nSearchType;
}

void SPHSolver::loadConfig(const std::string &file) {
  if (checkInited()) return;

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: Loading SPH parameters from config file: " << file << std::endl;
  #endif

  // Read JSON file
  Json::Reader reader;
  Json::Value root;
  std::ifstream sceneStream(file, std::ifstream::binary);

  bool success = reader.parse(sceneStream, root, false);
  if (!success) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_ERROR
    std::cerr << "ERROR: Failed to parse config file " << file << std::endl;
    #endif
    return;
  }

  kStiffness = root["sph"].get("kStiffness", SPHConfig_Default_kStiffness).asFloat();
  muViscosity = root["sph"].get("muViscosity", SPHConfig_Default_muViscosity).asFloat();
  mMass = root["sph"].get("mMass", SPHConfig_Default_mMass).asFloat();
  dRestDensity = root["sph"].get("dRestDensity", SPHConfig_Default_dRestDensity).asFloat();
  dtTimestep = root["sph"].get("dtTimestep", SPHConfig_Default_dtTimestep).asFloat();
  kernelRadius = root["sph"].get("kernelRadius", SPHConfig_Default_kernelRadius).asFloat();

  #if MFluidSolver_RECORD_PERFORMANCE
  int tempMaxUpdates = root.get("numUpdates", 0).asInt();
  if (tempMaxUpdates <= 0) {
    limitNumUpdates = false;
  } else {
    maxUpdates = tempMaxUpdates;
    limitNumUpdates = true;
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
    std::cout << "INFO: Limiting number of updates to " << maxUpdates << std::endl;
    #endif
  }
  #endif

  setFixedTimestep(dtTimestep);

  std::string neighborSearchTypeString = root["sph"].get("neighborSearchType", SPHConfig_Default_neighborSearchTypeString).asString();
  MUtils::toLowerInplace(neighborSearchTypeString);
  if (neighborSearchTypeString == "naive") {
    nSearchType = NeighborSearchType::Naive;
  } else if (neighborSearchTypeString == "uniform") {
    nSearchType = NeighborSearchType::UniformGrid;
  } else if (neighborSearchTypeString == "indexsorteduniform") {
    nSearchType = NeighborSearchType::IndexSortedUniformGrid;
  } else if (neighborSearchTypeString == "zindexsorteduniform") {
    nSearchType = NeighborSearchType::ZIndexSortedUniformGrid;
  }

  std::string visualizationTypeString = root.get("visualization", MFluidSolver_DEFAULT_VISUALIZATION_STRING).asString();
  MUtils::toLowerInplace(visualizationTypeString);
  if (visualizationTypeString == "neighbors") {
    visualizationType = FluidVisualizationType::Neighbors;
  } else if (visualizationTypeString == "none") {
    visualizationType = FluidVisualizationType::None;
  } else if (visualizationTypeString == "velocity") {
    visualizationType = FluidVisualizationType::Velocity;
  }

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: - Stiffness: " << kStiffness << std::endl;
  std::cout << "INFO: - Viscosity: " << muViscosity << std::endl;
  std::cout << "INFO: - Mass: " << mMass << std::endl;
  std::cout << "INFO: - Rest Density: " << dRestDensity << std::endl;
  std::cout << "INFO: - Timestep: " << dtTimestep << std::endl;
  std::cout << "INFO: - Kernel Radius: " << kernelRadius << std::endl;
  #endif
}

void SPHSolver::update(double deltaT) {
  #if MFluidSolver_RECORD_PERFORMANCE
  if (limitNumUpdates && numUpdates >= maxUpdates) {
    return;
  }
  std::clock_t startTime = std::clock();
  #endif

  // NOTE: TIMESTEP IS OVERWRITTEN HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  deltaT = _fixedTimestep;

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_DEBUG
  std::cout << "DEBUG: Updating by " << deltaT << " seconds" << std::endl;
  #endif

  // Prepare neighbor search
  if (nSearchType == NeighborSearchType::UniformGrid) {
    nSearch->clear();
    for (SPHParticle &p : _particles) {
      nSearch->addParticle(&p);
    }
    /*for (SPHParticle &p : _particles) {
      nSearch->updateParticle(p);
    }*/
  } else if (nSearchType == NeighborSearchType::IndexSortedUniformGrid || nSearchType == NeighborSearchType::ZIndexSortedUniformGrid) {
    IndexSortedUniformGridNeighborSearch *isugSearch = static_cast<IndexSortedUniformGridNeighborSearch *>(nSearch);
    isugSearch->isuGrid->resetAndFillCells();
  }

  // Calculate neighbors
  for (SPHParticle &p : _particles) {
    nSearch->findNeighbors(&p);
  }

  // Compute density and pressure
  for (SPHParticle &p : _particles) {
    // Density
    float densitySum = p.mass() * kernelFunctions.computePoly6(glm::vec3(0));
    if (p.neighbors()->size() > 0) {
      for (SPHParticle *n : *(p.neighbors())) {
        densitySum += n->mass() * kernelFunctions.computePoly6(p.position() - n->position());
      }
    }
    p.setDensity(densitySum);

    // Pressure
    float pressureTemp = kStiffness * (p.density() - dRestDensity);
    if (pressureTemp < 0) pressureTemp = 0;
    p.setPressure(pressureTemp);
  }

  // Compute forces
  for (SPHParticle &p : _particles) {
    glm::vec3 pressureFD(0);
    glm::vec3 viscosityFD(0);
    glm::vec3 gravityFD = glm::vec3(0, p.density() * _gravity, 0);

    for (SPHParticle *n : *(p.neighbors())) {
      pressureFD -= n->mass() / n->density() *
        (p.pressure() + n->pressure()) / 2.f *
        kernelFunctions.computeSpikyGradient(p.position() - n->position());
    }

    if (muViscosity > 0) {
      for (SPHParticle *n : *(p.neighbors())) {
        viscosityFD += glm::vec3(n->mass() / n->density() *
          (n->velocity() - p.velocity()) *
          (float)kernelFunctions.computeViscousLaplacian(p.position() - n->position()));
      }

      viscosityFD *= muViscosity;
    }

    p.setForceDensity(pressureFD + viscosityFD + gravityFD);
  }

  // Compute velocity and position, check bounds
  for (SPHParticle &p : _particles) {
    p.update(deltaT);

    // Check bounds
    glm::ivec3 violations;
    if (!fluidContainer->intersects(p.position(), violations)) {
      glm::vec3 position = p.position();
      p.reverseVelocity(violations, 0.1f); // Bounce

      // TODO: Generalize
      glm::vec3 scaleVec = fluidContainer->transform.scale();
      glm::vec3 minBounds = -0.5f * scaleVec;
      glm::vec3 maxBounds = 0.5f * scaleVec;

      if (violations.x < 0) position.x = minBounds.x;
      else if (violations.x > 0) position.x = maxBounds.x;

      if (violations.y < 0) position.y = minBounds.y;
      else if (violations.y > 0) position.y = maxBounds.y;

      if (violations.z < 0) position.z = minBounds.z;
      else if (violations.z > 0) position.z = maxBounds.z;

      p.setPosition(position);
    }

    if (visualizationType == FluidVisualizationType::Velocity) {
      p.color = glm::normalize(glm::abs(p.velocity()));
    }
  }

  #if MFluidSolver_RECORD_PERFORMANCE
  std::clock_t endTime = std::clock();
  double computeTimeInSeconds = (endTime - startTime) / (double) CLOCKS_PER_SEC;
  ++numUpdates;
  computeTime += computeTimeInSeconds;
  #endif
}

void SPHSolver::addParticleAt(const glm::vec3 &position) {
  if (_particles.size() < _maxParticles) {
    SPHParticle p(mMass, position);
    _particles.push_back(p);
    p.index = _particles.size();
    nSearch->addParticle(&p);
  }
}

std::vector<SPHParticle> &SPHSolver::particles() {
  return _particles;
}

unsigned int SPHSolver::numParticles() const {
  return _particles.size();
}

void SPHSolver::setParticleSeparation(float ps) {
  FluidSolver::setParticleSeparation(ps);
}

void SPHSolver::setMaxParticles(int mp) {
  FluidSolver::setMaxParticles(mp);
  if (mp <= 0) {
    _particles.clear();
  } else if (mp > _particles.size()) {
    while (_particles.size() > mp) {
      _particles.erase(_particles.end() - 1);
    }
  }
}

bool SPHSolver::checkInited() {
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_ERROR
  if (inited) {
    std::cerr << "ERROR: Already inited SPH Solver. Config will not be applied" << std::endl;
  }
  #endif

  return inited;
}

void SPHSolver::visualizeParticleNeighbors(SPHParticle *target) {
  if (visualizationType != FluidVisualizationType::Neighbors) return;

  for (SPHParticle &p : _particles) {
    p.color = glm::vec3(0, 0, 1);
  }

  if (nSearchType == NeighborSearchType::UniformGrid) {
    static_cast<UniformGridNeighborSearch *>(nSearch)->clear();
    for (SPHParticle &p : _particles) {
      nSearch->addParticle(&p);
    }
  } else if (nSearchType == NeighborSearchType::IndexSortedUniformGrid || nSearchType == NeighborSearchType::ZIndexSortedUniformGrid) {
    IndexSortedUniformGridNeighborSearch *isugSearch = static_cast<IndexSortedUniformGridNeighborSearch *>(nSearch);
    isugSearch->isuGrid->resetAndFillCells();
  }

  target->clearNeighbors();
  nSearch->findNeighbors(target);

  target->color = glm::vec3(1, 1, 0);
  for (SPHParticle *n : *(target->neighbors())) {
    n->color = glm::vec3(1, 0, 0);
  }
}

void SPHSolver::visualizeParticle0Neighbors() {
  visualizeParticleNeighbors(&(_particles.at(0)));
}

void SPHSolver::visualizeRandomParticlesNeighbors() {
  visualizeParticleNeighbors(&(_particles.at(rand() % _particles.size())));
}

#if MFluidSolver_USE_OPENVDB
void SPHSolver::exportVDB() {
  if (nSearchType == NeighborSearchType::UniformGrid) {
    static_cast<UniformGridNeighborSearch *>(nSearch)->exportVDB();
  } else {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
    std::cout << "WARN: Cannot export VDB unless using grid neighbor search" << std::endl;
    #endif
  }
}
#endif
