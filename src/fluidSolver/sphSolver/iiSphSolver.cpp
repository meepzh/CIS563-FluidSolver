//
//  iiSphSolver.cpp
//  MFluidSolver

#include "iiSphSolver.hpp"

#include <cassert>
#include <cmath>

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
  iter_all_sphparticles_end

  iter_all_sphparticles_start
    calculateNonPressureForce(p);

    // Calculate intermediate velocity
    p.setVelocity(p.velocity() + p.nonPressureForce() / p.mass() * deltaTF);
    assert(!std::isnan(p.velocity().x));

    // Calculate self displacement
    glm::vec3 d(0);
    float density2 = p.density() * p.density();
    for (SPHParticle *n : *(p.neighbors())) {
      d -= n->mass() / density2 * kernelFunctions.computeSpikyGradient(p.position() - n->position());
    }
    p.setDSelf(d * deltaTF2);
    assert(!std::isnan(p.dSelf().x));
  iter_all_sphparticles_end

  iter_all_sphparticles_start
    // Set intermediate density and aSelf
    float aTemp = 0;
    float densityNTemp = 0;
    for (SPHParticle *n : *(p.neighbors())) {
      glm::vec3 spikyTemp = kernelFunctions.computeSpikyGradient(p.position() - n->position());

      densityNTemp += n->mass() *
        glm::dot(p.velocity() - n->velocity(), spikyTemp);

      glm::vec3 dJI = -1 * p.mass() / (n->density() * n->density()) * spikyTemp;
      dJI *= deltaTF2;

      aTemp += n->mass() * glm::dot(p.dSelf() - dJI, kernelFunctions.computeSpikyGradient(p.position() - n->position()));
    }
    assert(!std::isnan(densityNTemp));
    p.setDensityIntermediate(p.density() + densityNTemp * deltaTF);
    assert(!std::isnan(aTemp));
    p.setASelf(aTemp);

    // Set iteration=0 pressure
    assert(!std::isnan(p.pressure()));
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
        displacementNTemp -= n->mass() / density2 * n->pressure() *
          kernelFunctions.computeSpikyGradient(p.position() - n->position());
      }
      p.setDNeighbors(displacementNTemp);
    iter_all_sphparticles_end

    iter_all_sphparticles_start
      // Calculate next pressure
      const float omega = 0.5f;
      float dTemp = 0;
      float nTemp = 0;
      for (SPHParticle *n : *(p.neighbors())) {
        glm::vec3 spikyTemp = kernelFunctions.computeSpikyGradient(n->position() - p.position());

        glm::vec3 dJI = -1 * p.mass() / (n->density() * n->density()) * spikyTemp;
        dJI *= deltaTF2;

        dTemp += n->mass() * glm::dot(p.dSelf() - dJI, spikyTemp);
        nTemp += n->mass() * glm::dot(p.dNeighbors() - n->dSelf() - n->dNeighbors() + dJI * p.pressure(), spikyTemp);
      }
      assert(!std::isnan(dTemp));
      assert(!std::isnan(nTemp));
      float pressureTemp = dRestDensity - p.density() - nTemp;
      pressureTemp *= omega / p.aSelf();
      pressureTemp += (1.f - omega) * p.pressure();

      // Sum for average density
      avgDensity += p.density() + p.pressure() * dTemp + nTemp;

      assert(!std::isnan(pressureTemp));
      p.setPressure(pressureTemp);
    iter_all_sphparticles_end

    avgDensity /= _particles.size();
    ++iteration;
  }

  // Procedure: Iteration
  iter_all_sphparticles_start
    calculatePressureForce(p);
  iter_all_sphparticles_end

  iter_all_sphparticles_start
    // Update
    glm::vec3 newVel = p.velocity() + p.pressureForce() / p.mass() * deltaTF;
    glm::vec3 newPos = p.position() + newVel * deltaTF;
    p.update(newVel, newPos);

    enforceBounds(p);
    visualizeParticle(p);
  iter_all_sphparticles_end
}