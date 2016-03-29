//
//  sphSolver.cpp
//  MFluidSolver

#include "sphSolver.hpp"

#include <cstdio>
#include <fstream>
#include <json/json.h>

SPHSolver::SPHSolver()
 : nSearch(nullptr), inited(false) {
  setDefaultConfig();
}

SPHSolver::~SPHSolver() {
  delete nSearch;
  for (SPHParticle *p : _particles) {
    delete p;
  }
}

void SPHSolver::init(const glm::vec3 &gridMin, const glm::vec3 &gridMax) {
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::printf("INFO: Initializing SPH Solver\n");
  #endif

  kernelFunctions.setKernelRadius(kernelRadius);

  switch (nSearchType) {
    case NeighborSearchType::Naive:
      #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
      std::printf("INFO: SPH Solver using naive neighbor search\n");
      #endif

      nSearch = new NaiveNeighborSearch();
      break;
    case NeighborSearchType::StandardGrid:
    default:
      #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
      std::printf("INFO: SPH Solver using uniform grid neighbor search\n");
      #endif

      // Note: Assumes grid cell size is equal to kernelRadius
      nSearch = new StandardGridNeighborSearch(kernelRadius, gridMin, gridMax, kernelRadius);
      break;
  }

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  if (muViscosity <= 0) {
    std::printf("INFO: As per config, viscosity is disabled.\n");
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
  std::printf("INFO: Loading SPH parameters from config file: %s\n", file.c_str());
  #endif

  // Read JSON file
  Json::Reader reader;
  Json::Value root;
  std::ifstream sceneStream(file, std::ifstream::binary);

  bool success = reader.parse(sceneStream, root, false);
  if (!success) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_ERROR
    std::fprintf(stderr, "ERROR: Failed to parse config file %s", file.c_str());
    #endif
    return;
  }

  kStiffness = root["sph"].get("kStiffness", SPHConfig_Default_kStiffness).asFloat();
  muViscosity = root["sph"].get("muViscosity", SPHConfig_Default_muViscosity).asFloat();
  mMass = root["sph"].get("mMass", SPHConfig_Default_mMass).asFloat();
  dRestDensity = root["sph"].get("dRestDensity", SPHConfig_Default_dRestDensity).asFloat();
  dtTimestep = root["sph"].get("dtTimestep", SPHConfig_Default_dtTimestep).asFloat();
  kernelRadius = root["sph"].get("kernelRadius", SPHConfig_Default_kernelRadius).asFloat();
  bool useUniformGrid = root["sph"].get("useUniformGrid", SPHConfig_Default_useUniformGrid).asBool();
  if (useUniformGrid) {
    nSearchType = NeighborSearchType::StandardGrid;
  } else {
    nSearchType = NeighborSearchType::Naive;
  }

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::printf("- INFO: Stiffness: %.2f\n", kStiffness);
  std::printf("- INFO: Viscosity: %.2f\n", muViscosity);
  std::printf("- INFO: Mass: %.2f\n", mMass);
  std::printf("- INFO: Rest Density: %.2f\n", dRestDensity);
  std::printf("- INFO: Timestep: %.2f\n", dtTimestep);
  std::printf("- INFO: Kernel Radius: %.2f\n", kernelRadius);
  #endif
}

void SPHSolver::update(double deltaT) {
  deltaT = MFluidSolver_DEFAULT_UPDATE_STEP;

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_DEBUG
  std::printf("DEBUG: Updating by %.4f seconds\n", deltaT);
  #endif

  // Prepare neighbor search
  if (nSearchType == NeighborSearchType::StandardGrid) {
    static_cast<StandardGridNeighborSearch *>(nSearch)->clear();
    for (SPHParticle *p : _particles) {
      nSearch->addParticle(p);
    }
  }

  // Calculate neighbors
  for (SPHParticle *p : _particles) {
    p->clearNeighbors();
    nSearch->findNeighbors(p);
  }

  // Compute density and pressure
  for (SPHParticle *p : _particles) {
    // Density
    float densitySum = p->mass() * kernelFunctions.computePoly6(glm::vec3(0));
    if (p->neighbors()->size() > 0) {
      for (SPHParticle *n : *(p->neighbors())) {
        densitySum += n->mass() * kernelFunctions.computePoly6(p->position() - n->position());
      }
    }
    p->setDensity(densitySum);

    // Pressure
    float pressureTemp = kStiffness * (p->density() - dRestDensity);
    if (pressureTemp < 0) pressureTemp = 0;
    p->setPressure(pressureTemp);
  }

  // Compute forces
  for (SPHParticle *p : _particles) {
    glm::vec3 pressureFD(0);
    glm::vec3 viscosityFD(0);
    glm::vec3 gravityFD = glm::vec3(0, p->density() * _gravity, 0);

    for (SPHParticle *n : *(p->neighbors())) {
      pressureFD -= n->mass() / n->density() *
        (p->pressure() + n->pressure()) / 2.f *
        kernelFunctions.computeSpikyGradient(p->position() - n->position());
    }

    if (muViscosity > 0) {
      for (SPHParticle *n : *(p->neighbors())) {
        viscosityFD += glm::vec3(n->mass() / n->density() *
          (n->velocity() - p->velocity()) *
          (float)kernelFunctions.computeViscousLaplacian(p->position() - n->position()));
      }

      viscosityFD *= muViscosity;
    }

    p->setForceDensity(pressureFD + viscosityFD + gravityFD);
  }

  // Compute velocity and position, check bounds
  for (SPHParticle *p : _particles) {
    p->update(deltaT);

    // Check bounds
    glm::ivec3 violations;
    if (!fluidContainer->intersects(p->position(), violations)) {
      glm::vec3 position = p->position();
      p->reverseVelocity(violations, 0.1f); // Bounce

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

      p->setPosition(position);
    }

    p->color = glm::normalize(glm::abs(p->velocity()));
  }
}

void SPHSolver::addParticleAt(const glm::vec3 &position) {
  if (_particles.size() < maxParticles) {
    SPHParticle *p = new SPHParticle(mMass, position);
    _particles.push_back(p);
    nSearch->addParticle(p);
  }
}

std::vector<SPHParticle *> &SPHSolver::particles() {
  return _particles;
}

unsigned int SPHSolver::numParticles() const {
  return _particles.size();
}

void SPHSolver::setParticleSeparation(float ps) {
  FluidSolver::setParticleSeparation(ps);
}

bool SPHSolver::checkInited() {
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_ERROR
  if (inited) {
    std::fprintf(stderr, "ERROR: Already inited SPH Solver. Config will not be applied");
  }
  #endif

  return inited;
}

void SPHSolver::demoCode(SPHParticle *target) {
  if (nSearchType == NeighborSearchType::StandardGrid) {
    static_cast<StandardGridNeighborSearch *>(nSearch)->clear();
    for (SPHParticle *p : _particles) {
      nSearch->addParticle(p);
    }
  }

  target->clearNeighbors();
  nSearch->findNeighbors(target);

  target->color = glm::vec3(1, 1, 0);
  for (SPHParticle *n : *(target->neighbors())) {
    n->color = glm::vec3(1, 0, 0);
  }
}

void SPHSolver::initialDemo() {
  demoCode(_particles.at(0));
}

void SPHSolver::randomDemo() {
  for (SPHParticle *p : _particles) {
    p->color = glm::vec3(0, 0, 1);
  }
  demoCode(_particles.at(rand() % _particles.size()));
}

void SPHSolver::exportVDB() {
  if (nSearchType == NeighborSearchType::StandardGrid) {
    static_cast<StandardGridNeighborSearch *>(nSearch)->exportVDB();
  } else {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
    std::printf("WARN: Cannot export VDB unless using grid neighbor search\n");
    #endif
  }
}
