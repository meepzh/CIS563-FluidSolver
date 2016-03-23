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

SPHConfig *SPHSolver::init(const double &kernelRadius,
    const glm::vec3 &gridMin, const glm::vec3 &gridMax, NeighborSearchType nSearchType) {
  kernelFunctions.setKernelRadius(kernelRadius);

  switch (nSearchType) {
    case NeighborSearchType::Naive:
      nSearch = new NaiveNeighborSearch();
      break;
    case NeighborSearchType::StandardGrid:
    default:
      nSearch = new StandardGridNeighborSearch(kernelRadius, gridMin, gridMax);
      break;
  }

  return &config;
}

void SPHSolver::setDefaultConfig() {
  config.kStiffness = SPHConfig_Default_kStiffness;
  config.muViscosity = SPHConfig_Default_muViscosity;
  config.rRadius = SPHConfig_Default_rRadius;
  config.mMass = SPHConfig_Default_mMass;
  config.dRestDensity = SPHConfig_Default_dRestDensity;
  config.dtTimestep = SPHConfig_Default_dtTimestep;
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

  config.kStiffness = root["sph"].get("kStiffness", SPHConfig_Default_kStiffness).asFloat();
  config.muViscosity = root["sph"].get("muViscosity", SPHConfig_Default_muViscosity).asFloat();
  config.rRadius = root["sph"].get("rRadius", SPHConfig_Default_rRadius).asFloat();
  config.mMass = root["sph"].get("mMass", SPHConfig_Default_mMass).asFloat();
  config.dRestDensity = root["sph"].get("dRestDensity", SPHConfig_Default_dRestDensity).asFloat();
  config.dtTimestep = root["sph"].get("dtTimestep", SPHConfig_Default_dtTimestep).asFloat();
}

void SPHSolver::update(double deltaT) {
  // Calculate neighbors
  for (SPHParticle *p : _particles) {
    p->clearNeighbors();
    nSearch->findNeighbors(p);
  }

  // Compute density and pressure
  for (SPHParticle *p : _particles) {

  }

  for (SPHParticle *p : _particles) {

  }

  for (SPHParticle *p : _particles) {

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