//
//  iiSphSolver.cpp
//  MFluidSolver

#include "iiSphSolver.hpp"

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

    // Calculate self displacement
    glm::vec3 d(0);
    float density2 = p.density() * p.density();
    for (SPHParticle *n : *(p.neighbors())) {
      d -= n->mass() / density2 * kernelFunctions.computeSpikyGradient(p.position() - n->position());
    }
    d *= deltaTF2;
  iter_all_sphparticles_end

  iter_all_sphparticles_start
    // Set intermediate density
    float densityNTemp = 0;
    for (SPHParticle *n : *(p.neighbors())) {
      densityNTemp += n->mass() *
        glm::dot(p.velocity() - n->velocity(), kernelFunctions.computeSpikyGradient(p.position() - n->position()));
    }
    p.setDensity(p.density() + densityNTemp * deltaTF);

    // Set iteration=0 pressure
    p.setPressure(0.5f * p.pressure()); // TODO: check if using correct pressure
  iter_all_sphparticles_end

  // Procedure: Pressure Solve
  unsigned int iteration = 0;
  float avgDensity = 0;
  const float eta = 0.01f * kernelRadius;
  // TODO: Figure out average density update
  while ((avgDensity - dRestDensity) > eta && iteration < 2) {
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
      float nTemp = 0;
      for (SPHParticle *n : *(p.neighbors())) {
        glm::vec3 dJI = -1 * p.mass() / (n->density() * n->density()) *
          kernelFunctions.computeSpikyGradient(n->position() - p.position());
        dJI *= deltaTF2;

        nTemp += n->mass() * glm::dot(p.dNeighbors() - n->dSelf() - n->dNeighbors() + dJI * p.pressure(),
          kernelFunctions.computeSpikyGradient(p.position() - n->position()));
      }
      nTemp = dRestDensity - p.density() - nTemp;
      nTemp *= omega / p.aSelf();
      nTemp += (1.f - omega) * p.pressure();
      p.setPressure(nTemp);

      // Sum for average density
      avgDensity += p.density();
    iter_all_sphparticles_end

    avgDensity /= _particles.size();
    ++iteration;
  }

  // Procedure: Iteration
  iter_all_sphparticles_start
    // Update
    glm::vec3 newVel = p.velocity() + p.pressureForce() / p.mass() * deltaTF;
    glm::vec3 newPos = p.position() + newVel * deltaTF;
    p.update(newVel, newPos);

    enforceBounds(p);
    visualizeParticle(p);
  iter_all_sphparticles_end
}