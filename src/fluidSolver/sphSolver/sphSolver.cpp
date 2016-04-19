//
//  sphSolver.cpp
//  MFluidSolver

#include "sphSolver.hpp"

#include <ctime>
#include <fstream>
#include <iostream>
#include <json/json.h>

#include "utils.hpp"

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
    case FluidVisualizationType::Density:
      std::cout << "INFO: Now visualizing density difference from rest density" << std::endl;
      break;
    case FluidVisualizationType::Index:
      std::cout << "INFO: Now visualizing index within particle array" << std::endl;
      break;
    case FluidVisualizationType::Neighbors:
      std::cout << "INFO: Now visualizing neighbors" << std::endl;
      break;
    case FluidVisualizationType::None:
      std::cout << "INFO: Visualization disabled" << std::endl;
      break;
    case FluidVisualizationType::Particle:
      std::cout << "INFO: Now highlighting particle ID " << targetParticle << std::endl;
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
  if (visualizationTypeString == "density") {
    visualizationType = FluidVisualizationType::Density;
  } else if (visualizationTypeString == "index") {
    visualizationType = FluidVisualizationType::Index;
  } else if (visualizationTypeString == "neighbors") {
    visualizationType = FluidVisualizationType::Neighbors;
  } else if (visualizationTypeString == "none") {
    visualizationType = FluidVisualizationType::None;
  } else if (visualizationTypeString == "particle") {
    visualizationType = FluidVisualizationType::Particle;
  } else if (visualizationTypeString == "pressure") {
    visualizationType = FluidVisualizationType::Pressure;
  } else if (visualizationTypeString == "velocity") {
    visualizationType = FluidVisualizationType::Velocity;
  } else if (visualizationTypeString == "velocitydir") {
    visualizationType = FluidVisualizationType::VelocityDir;
  }
  visualizationMaxDensityDifference = root["visualization"].get("maxDensityDifference", MFluidSolver_DEFAULT_VISUALIZATION_MAXDENSITYDIFFERENCE).asFloat();
  visualizationMaxPressure = root["visualization"].get("maxPressure", MFluidSolver_DEFAULT_VISUALIZATION_MAXPRESSURE).asFloat();
  visualizationMaxVelocity = root["visualization"].get("maxVelocity", MFluidSolver_DEFAULT_VISUALIZATION_MAXVELOCITY).asFloat();
  targetParticle = root["visualization"].get("particleId", 0).asInt();
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
  const float deltaTF = (float)deltaT;

  logTimestep();
  prepNeighborSearch();
  runNeighborSearch();

  // Compute density and pressure
  iter_all_sphparticles_start
    calculateDensity(p);

    // Pressure
    double densityRatio = p.density() / dRestDensity;
    double pressureTemp = kStiffness * dRestDensity / 7.f * (
      densityRatio * densityRatio * densityRatio * densityRatio * densityRatio * densityRatio * densityRatio - 1.f);
    if (pressureTemp < 0) pressureTemp = 0;
    p.setPressure(pressureTemp);
  iter_all_sphparticles_end

  // Compute forces
  iter_all_sphparticles_start
    calculateNonPressureForce(p);
    calculatePressureForce(p);
  iter_all_sphparticles_end

  // Compute velocity and position, check bounds
  iter_all_sphparticles_start
    // Update
    glm::vec3 newVel = p.velocity() + p.force() / p.mass() * deltaTF;
    glm::vec3 newPos = p.position() + newVel * deltaTF;
    p.update(newVel, newPos);

    enforceBounds(p);

    largestIndex = _particles.back().index;
    visualizeParticle(p);
  iter_all_sphparticles_end
}

// Particles
void SPHSolver::addParticleAt(const glm::vec3 &position) {
  if (_particles.size() < _maxParticles) {
    SPHParticle pStack(mMass, position);
    _particles.push_back(pStack);
    SPHParticle *pList = &(_particles.back());
    pList->index = _particles.size() - 1;
    nSearch->addParticle(pList);
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
void SPHSolver::prepNeighborSearchAfterSceneLoad() {
  // Initially sort particles with standard sort
  if (nSearchType == NeighborSearchType::IndexSortedUniformGridWithInsertion || nSearchType == NeighborSearchType::ZIndexSortedUniformGridWithInsertion) {
    IndexSortedUniformGridNeighborSearch *isugSearch = static_cast<IndexSortedUniformGridNeighborSearch *>(nSearch);
    isugSearch->isuGrid->resetAndFillCells(true);
  }
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

void SPHSolver::visualizeRandomParticlesNeighbors() {
  if (_particles.size() > 0) {
    visualizeParticleNeighbors(&(_particles[rand() % _particles.size()]));
  }
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
  else {
    std::cout << "WARN: No particles available for neighbor visualization" << std::endl;
  }
  #endif
}

// Misc
void SPHSolver::initVisualization() {
  if (visualizationType == FluidVisualizationType::Neighbors) {
    if (_particles.size() > 0) {
      visualizeParticleNeighbors(&(_particles[0]));
    }
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
    else {
      std::cout << "WARN: No particles available for neighbor visualization" << std::endl;
    }
    #endif
  } else if (visualizationType == FluidVisualizationType::Particle) {
    if (_particles.size() > targetParticle) {
      _particles[targetParticle].color = glm::vec3(1, 1, 0);
    }
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
    else {
      std::cout << "WARN: Target particle " << targetParticle << " not available for visualization" << std::endl;
    }
    #endif
  }
}

void SPHSolver::printPerformanceStats() {
  FluidSolver::printPerformanceStats();
  nSearch->printPerformanceStats();
}

void SPHSolver::sceneLoaded() {
  initVisualization();
  prepNeighborSearchAfterSceneLoad();
}

#if MFluidSolver_USE_PARTIO
void SPHSolver::exportBgeo() {
  // http://www.disneyanimation.com/technology/partio.html
  // http://wdas.github.io/partio/doxygen/
  Partio::ParticlesDataMutable *particleData = Partio::create();
  Partio::ParticlesDataMutable::iterator iterator = particleData->addParticles(_particles.size());
  Partio::ParticleAttribute posAttr = particleData->addAttribute("position", Partio::ParticleAttributeType::VECTOR, 3);
  Partio::ParticleAccessor posAcc(posAttr);
  iterator.addAccessor(posAcc);

  unsigned int idx = 0;
  for (Partio::ParticlesDataMutable::iterator it = particleData->begin(); it != particleData->end(); ++it) {
    float *vectorData = posAcc.raw<float>(it);
    const glm::vec3 position = _particles.at(idx).position();
    for (unsigned int i = 0; i < 3; ++i) {
      vectorData[i] = position[i];
    }
    ++idx;
  }

  Partio::write("test.bgeo",*particleData);
  particleData->release();
}
#endif

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