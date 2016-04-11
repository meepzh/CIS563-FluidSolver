//
//  sphSolver.inline.hpp
//  MFluidSolver

#include <cassert>
#include <cmath>

#if MFluidSolver_USE_TBB
  #define iter_all_sphparticles_start tbb::parallel_for(tbb::blocked_range<size_t>(0, _particles.size()), \
    [&](const tbb::blocked_range<size_t> &r) { \
      for (unsigned int i = r.begin(); i != r.end(); ++i) { \
        SPHParticle &p = _particles.at(i);
  #define iter_all_sphparticles_end }});
#else
  #define iter_all_sphparticles_start for (unsigned int i = 0; i < _particles.size(); ++i) { \
    SPHParticle &p = _particles.at(i);
  #define iter_all_sphparticles_end }
#endif

inline void SPHSolver::calculateDensity(SPHParticle &p) {
  float densitySum = p.mass() * kernelFunctions.computePoly6(glm::vec3(0));
  if (p.neighbors()->size() > 0) {
    for (SPHParticle *n : *(p.neighbors())) {
      densitySum += n->mass() * kernelFunctions.computePoly6(p.position() - n->position());
    }
  }
  p.setDensity(densitySum);
}

inline void SPHSolver::calculateNonPressureForce(SPHParticle &p) {
  glm::vec3 viscosityForce(0);
  glm::vec3 gravityForce = glm::vec3(0, p.mass() * _gravity, 0);

  if (muViscosity > 0) {
    for (SPHParticle *n : *(p.neighbors())) {
      viscosityForce += glm::vec3(n->mass() *
        (n->velocity() - p.velocity()) *
        (float)kernelFunctions.computeViscousLaplacian(p.position() - n->position()));
    }
  }
  viscosityForce *= muViscosity / p.density() * p.mass();

  p.setNonPressureForce(viscosityForce + gravityForce);
}

inline void SPHSolver::calculatePressureForce(SPHParticle &p) {
  glm::vec3 pressureForce(0);
  float pDensity2 = p.density() * p.density();
  for (SPHParticle *n : *(p.neighbors())) {
    pressureForce += n->mass() *
      (p.pressure() / pDensity2 + n->pressure() / (n->density() * n->density())) *
      kernelFunctions.computeSpikyGradient(p.position() - n->position());
    assert(!std::isnan(pressureForce.x) && !std::isinf(pressureForce.x));
  }
  pressureForce *= -1 * p.mass();
  p.setPressureForce(pressureForce);
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
    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 0.05f; // Jitter

    if (violations.x < 0) position.x = minBounds.x + r;
    else if (violations.x > 0) position.x = maxBounds.x - r;

    if (violations.y < 0) position.y = minBounds.y + r;
    else if (violations.y > 0) position.y = maxBounds.y - r;

    if (violations.z < 0) position.z = minBounds.z + r;
    else if (violations.z > 0) position.z = maxBounds.z - r;

    p.setPosition(position);
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

inline void SPHSolver::runNeighborSearch() {
  iter_all_sphparticles_start
    nSearch->findNeighbors(&p);
  iter_all_sphparticles_end
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