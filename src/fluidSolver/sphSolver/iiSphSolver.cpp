//
//  iiSphSolver.cpp
//  MFluidSolver

#include "iiSphSolver.hpp"

#include <cassert>
#include <cmath>

#define checkValidNumber(X) assert(!std::isnan(X) && !std::isinf(X))

void IISPHSolver::update(double deltaT) {
  if (checkIfEnded()) return;

  // NOTE: TIMESTEP IS OVERWRITTEN HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  deltaT = _fixedTimestep;
  const float deltaTF = (float)deltaT;
  const float deltaTF2 = (float)(deltaT * deltaT);

  logTimestep();
  prepNeighborSearch();
  runNeighborSearch();

  // Procedure: Predict Advection
  iter_all_sphparticles_start
    calculateDensity(p);

    calculateNonPressureForce(p);

    // Calculate intermediate velocity
    p.setVelocityIntermediate(p.velocity() + p.nonPressureForce() / p.mass() * deltaTF);
    checkValidNumber(p.velocityIntermediate().x);

    // Calculate self displacement
    glm::vec3 d(0);
    float density2 = p.density() * p.density();
    for (SPHParticle *n : *(p.neighbors())) {
      d -= n->mass() / density2 * kernelFunctions.computeSpikyGradient(p.position() - n->position());
    }
    p.setDSelf(d * deltaTF2);
    checkValidNumber(p.dSelf().x);
  iter_all_sphparticles_end

  iter_all_sphparticles_start
    // Set intermediate density and aSelf
    float aTemp = 0;
    float densityNTemp = 0;
    for (SPHParticle *n : *(p.neighbors())) {
      const glm::vec3 spikyTemp = kernelFunctions.computeSpikyGradient(p.position() - n->position());

      glm::vec3 dJI = -1 * p.mass() / (p.density() * p.density()) * spikyTemp;
      dJI *= deltaTF2;

      densityNTemp += n->mass() *
        glm::dot(p.velocityIntermediate() - n->velocityIntermediate(), spikyTemp);

      aTemp += n->mass() * glm::dot(p.dSelf() - dJI,
        kernelFunctions.computeSpikyGradient(n->position() - p.position()));
    }
    checkValidNumber(densityNTemp);
    p.setDensityIntermediate(p.density() + densityNTemp * deltaTF);
    checkValidNumber(aTemp);
    if (p.neighbors()->size() == 0) aTemp = 1.f;
    p.setASelf(aTemp);

    // Set iteration=0 pressure
    checkValidNumber(p.pressure());
    p.setPressure(0.5f * p.pressure()); // TODO: check if using correct pressure
  iter_all_sphparticles_end

  // Procedure: Pressure Solve
  unsigned int iteration = 0;
  float avgDensity = 0;
  const float eta = 0.01f * kernelRadius;
  while ((avgDensity - dRestDensity) > eta || iteration < 2) {
    avgDensity = 0;

    iter_all_sphparticles_start
      // Calculate displacement due to neighbors
      glm::vec3 displacementNTemp(0);
      float density2 = p.density() * p.density();
      for (SPHParticle *n : *(p.neighbors())) {
        displacementNTemp -= n->mass() / (n->density() * n->density()) * n->pressure() *
          kernelFunctions.computeSpikyGradient(p.position() - n->position());
        checkValidNumber(displacementNTemp.x);
      }
      p.setDNeighbors(displacementNTemp);
      checkValidNumber(displacementNTemp.x);
    iter_all_sphparticles_end

    iter_all_sphparticles_start
      // Calculate next pressure
      const float omega = 0.5f;
      float dTemp = 0;
      float nTemp = 0;
      for (SPHParticle *n : *(p.neighbors())) {
        glm::vec3 spikyTemp = kernelFunctions.computeSpikyGradient(n->position() - p.position());

        glm::vec3 dJI = -1 * p.mass() / (p.density() * p.density()) * spikyTemp;
        dJI *= deltaTF2;

        dTemp += n->mass() * glm::dot(p.dSelf() - dJI, spikyTemp);
        nTemp += n->mass() * glm::dot(p.dNeighbors() - n->dSelf() - n->dNeighbors() + dJI * p.pressure(), spikyTemp);
        checkValidNumber(dTemp);
        checkValidNumber(nTemp);
      }
      float pressureTemp = dRestDensity - p.densityIntermediate() - nTemp;
      pressureTemp *= omega / p.aSelf();
      pressureTemp += (1.f - omega) * p.pressure();

      // Sum for average density
      avgDensity += p.densityIntermediate() + p.pressure() * dTemp + nTemp; // TODO: can't use parallel_for

      checkValidNumber(pressureTemp);
      p.setPressure(pressureTemp);
    iter_all_sphparticles_end

    checkValidNumber(avgDensity);
    avgDensity /= (float)_particles.size();
    ++iteration;
  }

  // Procedure: Iteration
  iter_all_sphparticles_start
    calculatePressureForce(p);
  iter_all_sphparticles_end

  iter_all_sphparticles_start
    // Update
    glm::vec3 newVel = p.velocityIntermediate() + p.pressureForce() / p.mass() * deltaTF;
    glm::vec3 newPos = p.position() + newVel * deltaTF;
    p.update(newVel, newPos);

    enforceBounds(p);
    visualizeParticle(p);
  iter_all_sphparticles_end
}
