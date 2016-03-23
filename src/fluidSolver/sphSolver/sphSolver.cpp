//
//  sphSolver.cpp
//  MFluidSolver

#include "sphSolver.hpp"

#include <cstdio>
#include <fstream>
#include <json/json.h>

SPHSolver::SPHSolver()
 : nSearch(nullptr) {
}

SPHSolver::~SPHSolver() {
  delete nSearch;
  for (SPHParticle *p : _particles) {
    delete p;
  }
}

void SPHSolver::init(const double &kernelRadius,
    const glm::vec3 &gridMin, const glm::vec3 &gridMax, NeighborSearchType nsType) {
  kernelFunctions.setKernelRadius(kernelRadius);
  nSearchType = nsType;

  switch (nSearchType) {
    case NeighborSearchType::Naive:
      nSearch = new NaiveNeighborSearch();
      break;
    case NeighborSearchType::StandardGrid:
    default:
      // Note: Assumes grid cell size is equal to kernelRadius
      nSearch = new StandardGridNeighborSearch(kernelRadius, gridMin, gridMax, kernelRadius);
      break;
  }
}

void SPHSolver::setDefaultConfig() {
  kStiffness = SPHConfig_Default_kStiffness;
  muViscosity = SPHConfig_Default_muViscosity;
  rRadius = SPHConfig_Default_rRadius;
  mMass = SPHConfig_Default_mMass;
  dRestDensity = SPHConfig_Default_dRestDensity;
  dtTimestep = SPHConfig_Default_dtTimestep;
}

void SPHSolver::loadConfig(const std::string &file) {
  printf("INFO: Loading SPH data from config file: %s\n", file.c_str());

  // Read JSON file
  Json::Reader reader;
  Json::Value root;
  std::ifstream sceneStream(file, std::ifstream::binary);

  bool success = reader.parse(sceneStream, root, false);
  if (!success) {
    std::fprintf(stderr, "ERROR: Failed to parse config file %s", file.c_str());
    return;
  }

  kStiffness = root["sph"].get("kStiffness", SPHConfig_Default_kStiffness).asFloat();
  muViscosity = root["sph"].get("muViscosity", SPHConfig_Default_muViscosity).asFloat();
  rRadius = root["sph"].get("rRadius", SPHConfig_Default_rRadius).asFloat();
  mMass = root["sph"].get("mMass", SPHConfig_Default_mMass).asFloat();
  dRestDensity = root["sph"].get("dRestDensity", SPHConfig_Default_dRestDensity).asFloat();
  dtTimestep = root["sph"].get("dtTimestep", SPHConfig_Default_dtTimestep).asFloat();
}

void SPHSolver::update(double deltaT) {
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
    float densitySum = 0;
    for (SPHParticle *n : *(p->neighbors())) {
      densitySum += n->mass() * kernelFunctions.computePoly6(p->position() - n->position());
    }
    p->setDensity(densitySum);

    // Pressure
    p->setPressure(kStiffness * (p->pressure() - dRestDensity));
  }

  // Compute forces
  for (SPHParticle *p : _particles) {
    // TODO Later
  }

  // Compute velocity and position
  for (SPHParticle *p : _particles) {
    p->update(deltaT);
  }
}

void SPHSolver::addParticleAt(const glm::vec3 &position) {
  if (_particles.size() < maxParticles) {
    SPHParticle *p = new SPHParticle(position);
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