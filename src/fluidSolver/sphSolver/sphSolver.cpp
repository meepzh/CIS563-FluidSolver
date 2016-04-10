//
//  sphSolver.cpp
//  MFluidSolver

#include "sphSolver.hpp"

#include <ctime>
#include <fstream>
#include <iostream>
#include <json/json.h>

#include "utils.hpp"

#if MFluidSolver_USE_TBB
  #include <tbb/parallel_for.h>
  #define iter_all_particles_start tbb::parallel_for(tbb::blocked_range<size_t>(0, _particles.size()), \
    [&](const tbb::blocked_range<size_t> &r) { \
      for (unsigned int i = r.begin(); i != r.end(); ++i) {
  #define iter_all_particles_end }});
#else
  #define iter_all_particles_start for (unsigned int i = 0; i < _particles.size(); ++i) {
  #define iter_all_particles_end }
#endif

// Constructor / Destructor
SPHSolver::SPHSolver()
 : nSearch(nullptr), inited(false) {
  setDefaultConfig();
}

SPHSolver::~SPHSolver() {
  delete nSearch;
}

// Configuration
void SPHSolver::init(const glm::vec3 &gridMin, const glm::vec3 &gridMax) {
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: Initializing SPH Solver" << std::endl;
  #endif

  kernelFunctions.setKernelRadius(kernelRadius);

  // Initialize neighbor search
  // Note: Neighbor search assumes grid cell size is equal to kernelRadius
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
    case NeighborSearchType::ZIndexSortedUniformGridWithInsertion:
      #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
      std::cout << "INFO: SPH Solver using Z-curve index sorted uniform grid neighbor search with insertion sort" << std::endl;
      #endif
      nSearch = new IndexSortedUniformGridNeighborSearch(kernelRadius, gridMin, gridMax, kernelRadius, &_particles, true);
      break;
    case NeighborSearchType::UniformGrid:
      #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
      std::cout << "INFO: SPH Solver using uniform grid neighbor search" << std::endl;
      #endif
      nSearch = new UniformGridNeighborSearch(kernelRadius, gridMin, gridMax, kernelRadius);
      break;
    case NeighborSearchType::IndexSortedUniformGridWithInsertion:
    default:
      #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
      std::cout << "INFO: SPH Solver using index sorted uniform grid neighbor search with insertion sort" << std::endl;
      #endif
      nSearch = new IndexSortedUniformGridNeighborSearch(kernelRadius, gridMin, gridMax, kernelRadius, &_particles, false);
      break;
  }

  // Print useful parameter info
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  switch (visualizationType) {
    case FluidVisualizationType::Neighbors:
      std::cout << "INFO: Now visualizing neighbors" << std::endl;
      break;
    case FluidVisualizationType::None:
      std::cout << "INFO: Visualization disabled" << std::endl;
      break;
    case FluidVisualizationType::Pressure:
      std::cout << "INFO: Now visualizing pressure" << std::endl;
      break;
    case FluidVisualizationType::Velocity:
      std::cout << "INFO: Now visualizing scalar velocity" << std::endl;
      break;
    case FluidVisualizationType::VelocityDir:
      std::cout << "INFO: Now visualizing normalized velocity direction" << std::endl;
      break;
  }
  if (muViscosity <= 0) {
    std::cout << "INFO: As per config, viscosity is disabled" << std::endl;
  }
  #endif

  inited = true;
}

void SPHSolver::setDefaultConfig() {
  // Set default SPH parameters
  if (checkInited()) return;
  kStiffness = MFluidSolver_DEFAULT_SPH_STIFFNESS;
  muViscosity = MFluidSolver_DEFAULT_SPH_VISCOSITY;
  mMass = MFluidSolver_DEFAULT_SPH_MASS;
  dRestDensity = MFluidSolver_DEFAULT_SPH_RESTDENSITY;
  dtTimestep = MFluidSolver_DEFAULT_SPH_TIMESTEP;
  kernelRadius = MFluidSolver_DEFAULT_SPH_KERNELRADIUS;
  nSearchType = MFluidSolver_DEFAULT_SPH_NEIGHBORSEARCHTYPE;
}

void SPHSolver::loadConfig(const std::string &file) {
  if (checkInited()) return;

  logTimestep();

  // Parse JSON file
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

  // Read SPH Parameters
  kStiffness = root["sph"].get("kStiffness", MFluidSolver_DEFAULT_SPH_STIFFNESS).asFloat();
  muViscosity = root["sph"].get("muViscosity", MFluidSolver_DEFAULT_SPH_VISCOSITY).asFloat();
  mMass = root["sph"].get("mMass", MFluidSolver_DEFAULT_SPH_MASS).asFloat();
  dRestDensity = root["sph"].get("dRestDensity", MFluidSolver_DEFAULT_SPH_RESTDENSITY).asFloat();
  dtTimestep = root["sph"].get("dtTimestep", MFluidSolver_DEFAULT_SPH_TIMESTEP).asFloat();
  kernelRadius = root["sph"].get("kernelRadius", MFluidSolver_DEFAULT_SPH_KERNELRADIUS).asFloat();
  setFixedTimestep(dtTimestep);

  // Read simulation time limits
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

  // Read neighbor search config
  std::string neighborSearchTypeString = root["sph"].get("neighborSearchType", MFluidSolver_DEFAULT_SPH_NEIGHBORSEARCHTYPE_STRING).asString();
  MUtils::toLowerInplace(neighborSearchTypeString);
  if (neighborSearchTypeString == "naive") {
    nSearchType = NeighborSearchType::Naive;
  } else if (neighborSearchTypeString == "uniform") {
    nSearchType = NeighborSearchType::UniformGrid;
  } else if (neighborSearchTypeString == "indexsorteduniform") {
    nSearchType = NeighborSearchType::IndexSortedUniformGrid;
  } else if (neighborSearchTypeString == "indexsorteduniforminsertionsort") {
    nSearchType = NeighborSearchType::IndexSortedUniformGridWithInsertion;
  } else if (neighborSearchTypeString == "zindexsorteduniform") {
    nSearchType = NeighborSearchType::ZIndexSortedUniformGrid;
  } else if (neighborSearchTypeString == "zindexsorteduniforminsertionsort") {
    nSearchType = NeighborSearchType::ZIndexSortedUniformGridWithInsertion;
  }

  // Read visualization config
  std::string visualizationTypeString = root["visualization"].get("type", MFluidSolver_DEFAULT_VISUALIZATION_STRING).asString();
  MUtils::toLowerInplace(visualizationTypeString);
  if (visualizationTypeString == "neighbors") {
    visualizationType = FluidVisualizationType::Neighbors;
  } else if (visualizationTypeString == "none") {
    visualizationType = FluidVisualizationType::None;
  } else if (visualizationTypeString == "pressure") {
    visualizationType = FluidVisualizationType::Pressure;
  } else if (visualizationTypeString == "velocity") {
    visualizationType = FluidVisualizationType::Velocity;
  } else if (visualizationTypeString == "velocitydir") {
    visualizationType = FluidVisualizationType::VelocityDir;
  }
  visualizationMaxPressure = root["visualization"].get("maxPressure", MFluidSolver_DEFAULT_VISUALIZATION_MAXPRESSURE).asFloat();
  visualizationMaxVelocity = root["visualization"].get("maxVelocity", MFluidSolver_DEFAULT_VISUALIZATION_MAXVELOCITY).asFloat();
  const Json::Value velocityColorArray = root["visualization"]["velocityColor"];
  if (!velocityColorArray.isNull()) {
    for (unsigned int i = 0; i < velocityColorArray.size() && i < 3; ++i) {
      visualizationVelocityColor[i] = (float)(velocityColorArray[i].asInt()) / 255.f;
      if (visualizationVelocityColor[i] > 1.f) visualizationVelocityColor[i] = 1.f;
    }
  }

  // Print parameters
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: - Stiffness: " << kStiffness << std::endl;
  std::cout << "INFO: - Viscosity: " << muViscosity << std::endl;
  std::cout << "INFO: - Mass: " << mMass << std::endl;
  std::cout << "INFO: - Rest Density: " << dRestDensity << std::endl;
  std::cout << "INFO: - Timestep: " << dtTimestep << std::endl;
  std::cout << "INFO: - Kernel Radius: " << kernelRadius << std::endl;
  #endif
}

// Solver!
void SPHSolver::update(double deltaT) {
  if (checkIfEnded()) return;

  // NOTE: TIMESTEP IS OVERWRITTEN HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  deltaT = _fixedTimestep;

  logTimestep();
  prepNeighborSearch();
  runNeighborSearch();

  // Compute density and pressure
  iter_all_particles_start
    SPHParticle &p = _particles.at(i);
    // Density
    calculateDensity(p);

    // Pressure
    float pressureTemp = kStiffness * (p.density() - dRestDensity);
    if (pressureTemp < 0) pressureTemp = 0;
    p.setPressure(pressureTemp);
  iter_all_particles_end

  // Compute forces
  iter_all_particles_start
    SPHParticle &p = _particles.at(i);
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
  iter_all_particles_end

  // Compute velocity and position, check bounds
  iter_all_particles_start
    SPHParticle &p = _particles.at(i);

    // Update
    glm::vec3 newVel = p.velocity() + p.forceDensity() / p.density() * (float)deltaT;
    glm::vec3 newPos = p.position() + newVel * (float)deltaT;
    p.update(newVel, newPos);

    enforceBounds(p);
    visualizeParticle(p);
  iter_all_particles_end
}

inline void SPHSolver::calculateDensity(SPHParticle &p) {
  float densitySum = p.mass() * kernelFunctions.computePoly6(glm::vec3(0));
  if (p.neighbors()->size() > 0) {
    for (SPHParticle *n : *(p.neighbors())) {
      densitySum += n->mass() * kernelFunctions.computePoly6(p.position() - n->position());
    }
  }
  p.setDensity(densitySum);
}

inline void SPHSolver::enforceBounds(SPHParticle &p) {
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
}

inline void SPHSolver::visualizeParticle(SPHParticle &p) {
  if (visualizationType == FluidVisualizationType::Pressure) {
    float pressureNormalized = p.pressure() / visualizationMaxPressure;
    if (pressureNormalized > 1.f) pressureNormalized = 1.f;
    p.color = pressureNormalized * glm::vec3(1.f);
  } else if (visualizationType == FluidVisualizationType::Velocity) {
    float velocityScalar = glm::length(p.velocity()) / visualizationMaxVelocity;
    if (velocityScalar > 1.f) velocityScalar = 1.f;
    p.color = (1 - velocityScalar) * visualizationVelocityColor + velocityScalar * glm::vec3(1.f);
  } else if (visualizationType == FluidVisualizationType::VelocityDir) {
    p.color = glm::normalize(glm::abs(p.velocity()));
  }
}

// Particles
void SPHSolver::addParticleAt(const glm::vec3 &position) {
  if (_particles.size() < _maxParticles) {
    SPHParticle p(mMass, position);
    _particles.push_back(p);
    p.index = _particles.size();
    nSearch->addParticle(&p);
  }
}

unsigned int SPHSolver::numParticles() const {
  return _particles.size();
}

std::vector<SPHParticle> &SPHSolver::particles() {
  return _particles;
}

void SPHSolver::setMaxParticles(int mp) {
  FluidSolver::setMaxParticles(mp);
  if (mp <= 0) {
    // Remove all particles as requested
    _particles.clear();
  } else if (mp > _particles.size()) {
    // Prune excess particles
    while (_particles.size() > mp) {
      _particles.erase(_particles.end() - 1);
    }
  }
}

void SPHSolver::setParticleSeparation(float ps) {
  FluidSolver::setParticleSeparation(ps);
}

// Neighbor Search
inline void SPHSolver::runNeighborSearch() {
  iter_all_particles_start
    SPHParticle &p = _particles.at(i);
    nSearch->findNeighbors(&p);
  iter_all_particles_end
}

void SPHSolver::prepNeighborSearchAfterSceneLoad() {
  // Initially sort particles with standard sort
  if (nSearchType == NeighborSearchType::IndexSortedUniformGridWithInsertion || nSearchType == NeighborSearchType::ZIndexSortedUniformGridWithInsertion) {
    IndexSortedUniformGridNeighborSearch *isugSearch = static_cast<IndexSortedUniformGridNeighborSearch *>(nSearch);
    isugSearch->isuGrid->resetAndFillCells(true);
  }
}

inline void SPHSolver::prepNeighborSearch() {
  if (nSearchType == NeighborSearchType::UniformGrid) {
    nSearch->clear();
    for (SPHParticle &p : _particles) {
      nSearch->addParticle(&p);
    }
  } else if (nSearchType == NeighborSearchType::IndexSortedUniformGrid || nSearchType == NeighborSearchType::ZIndexSortedUniformGrid) {
    IndexSortedUniformGridNeighborSearch *isugSearch = static_cast<IndexSortedUniformGridNeighborSearch *>(nSearch);
    isugSearch->isuGrid->resetAndFillCells(true);
  } else if (nSearchType == NeighborSearchType::IndexSortedUniformGridWithInsertion || nSearchType == NeighborSearchType::ZIndexSortedUniformGridWithInsertion) {
    IndexSortedUniformGridNeighborSearch *isugSearch = static_cast<IndexSortedUniformGridNeighborSearch *>(nSearch);
    isugSearch->isuGrid->resetAndFillCells(false); // Use insertion sort
  }
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_DEBUG
  else {
    std::cout << "DEBUG: No neighbor search prep being done" << std::endl;
  }
  #endif
}

// Neighbor Visualization
void SPHSolver::visualizeParticleNeighbors(SPHParticle *target) {
  if (visualizationType != FluidVisualizationType::Neighbors) return;

  // Reset color
  for (SPHParticle &p : _particles) {
    p.color = glm::vec3(0, 0, 1);
  }

  // Redo neighbor search
  prepNeighborSearch();
  target->clearNeighbors();
  nSearch->findNeighbors(target);

  // Color particles
  target->color = glm::vec3(1, 1, 0);
  for (SPHParticle *n : *(target->neighbors())) {
    n->color = glm::vec3(1, 0, 0);
  }
}

void SPHSolver::visualizeParticle0Neighbors() {
  if (_particles.size() > 0) {
    visualizeParticleNeighbors(&(_particles.at(0)));
  }
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
  else {
    std::cout << "WARN: No particles available for neighbor visualization" << std::endl;
  }
  #endif
}

void SPHSolver::visualizeRandomParticlesNeighbors() {
  if (_particles.size() > 0) {
    visualizeParticleNeighbors(&(_particles.at(rand() % _particles.size())));
  }
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
  else {
    std::cout << "WARN: No particles available for neighbor visualization" << std::endl;
  }
  #endif
}

// Misc
void SPHSolver::printPerformanceStats() {
  FluidSolver::printPerformanceStats();
  nSearch->printPerformanceStats();
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

// Initialization
bool SPHSolver::checkInited() {
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_ERROR
  if (inited) {
    std::cerr << "ERROR: Already inited SPH Solver. Config will not be applied" << std::endl;
  }
  #endif

  return inited;
}