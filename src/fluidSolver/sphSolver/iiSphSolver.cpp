//
//  iiSphSolver.cpp
//  MFluidSolver

#include "iiSphSolver.hpp"

#include <cassert>
#include <cmath>

#include "utils.hpp"

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

    // Calculate intermediate velocity v^{adv}_i
    p.setVelocityIntermediate(p.velocity() + p.nonPressureForce() / p.mass() * deltaTF);
    checkValidNumber(p.velocityIntermediate().x);

    // Calculate self displacement d_{ii}
    glm::vec3 dSelf(0);
    float pDensity2 = p.density() * p.density();
    for (SPHParticle *n : *(p.neighbors())) {
      dSelf -= n->mass() / pDensity2 * kernelFunctions.computeSpikyGradient(p.position() - n->position());
    }
    p.setDSelf(dSelf * deltaTF2);
    checkValidNumber(p.dSelf().x);
  iter_all_sphparticles_end

  iter_all_sphparticles_start
    // Set intermediate density and aSelf
    float aSelf = 0;
    float neighborDensityEstimate = 0;
    for (SPHParticle *n : *(p.neighbors())) {
      const glm::vec3 spikyIJ = kernelFunctions.computeSpikyGradient(p.position() - n->position());

      glm::vec3 dJI = -1 * p.mass() / (p.density() * p.density()) *
        kernelFunctions.computeSpikyGradient(n->position() - p.position());
      dJI *= deltaTF2;

      neighborDensityEstimate += n->mass() *
        glm::dot(p.velocityIntermediate() - n->velocityIntermediate(), spikyIJ);

      aSelf += n->mass() * glm::dot(p.dSelf() - dJI, spikyIJ);
    }
    checkValidNumber(neighborDensityEstimate);
    p.setDensityIntermediate(p.density() + neighborDensityEstimate * deltaTF);
    checkValidNumber(aSelf);
    /*if (aSelf == 0.f) {
      if (p.neighbors()->size() != 0) {
        std::cout << "FATAL: Particle has neighbors yet aSelf is zero" << std::endl;
        throw i;
      }
    }*/
    if (MUtils::fequal<float>(aSelf, 0.f, 0.001f)) {
      aSelf = 1.f;
    }
    p.setASelf(aSelf);

    // Set iteration=0 pressure p^0_i
    p.setPressure(0.5f * p.pressure());
    checkValidNumber(p.pressure());
  iter_all_sphparticles_end

  // Procedure: Pressure Solve
  unsigned int iteration = 0;
  float avgDensity = 0;
  const float eta = 0.01f * kernelRadius;
  while (((avgDensity - dRestDensity) > eta || iteration < 2) && iteration < 50) {
    avgDensity = 0;

    iter_all_sphparticles_start
      // Calculate displacement due to neighbors
      glm::vec3 dFromNeighborPressure(0);
      float density2 = p.density() * p.density();
      for (SPHParticle *n : *(p.neighbors())) {
        dFromNeighborPressure -= n->mass() * n->pressure() / (n->density() * n->density()) *
          kernelFunctions.computeSpikyGradient(p.position() - n->position());
        checkValidNumber(dFromNeighborPressure.x);
      }
      checkValidNumber(dFromNeighborPressure.x);
      p.setDNeighbors(dFromNeighborPressure);
    iter_all_sphparticles_end

    #if MFluidSolver_USE_TBB
    avgDensity = tbb::parallel_reduce(tbb::blocked_range<size_t>(0, _particles.size()), 0.f,
      [&](const tbb::blocked_range<size_t> &r, float partialDensitySum) {
        for (unsigned int i = r.begin(); i != r.end(); ++i) {
    #else
        for (unsigned int i = 0; i < _particles.size(); ++i) {
    #endif
          SPHParticle &p = _particles.at(i);
          if (p.aSelf() == 0.f) {
            p.setPressure(0); // TODO: Check what pressure to set
            continue;
          }

          // Calculate next pressure
          const float omega = 0.5f;
          float aSelfNew = 0;
          float aNeighbors = 0;
          for (SPHParticle *n : *(p.neighbors())) {
            glm::vec3 spikyIJ = kernelFunctions.computeSpikyGradient(p.position() - n->position());

            glm::vec3 dJI = -1 * p.mass() / (p.density() * p.density()) *
              kernelFunctions.computeSpikyGradient(n->position() - p.position());

            aSelfNew += n->mass() * glm::dot(p.dSelf() - dJI, spikyIJ);
            aNeighbors += n->mass() * glm::dot(p.dNeighbors() - n->dSelf() - n->dNeighbors() + dJI * p.pressure(), spikyIJ);
            checkValidNumber(aSelfNew);
            checkValidNumber(aNeighbors);
          }
          // Sum for average density
          // Note that density depends on old pressure
          float densityNew = p.densityIntermediate() + p.pressure() * aSelfNew + aNeighbors;
          p.setDensity(densityNew); // TODO: Check if we should do this

          // Update average density
          #if MFluidSolver_USE_TBB
            if (densityNew > dRestDensity) {
              partialDensitySum += densityNew;
            } else {
              partialDensitySum += dRestDensity;
            }
          #else
            if (densityNew > dRestDensity) {
              avgDensity += densityNew;
            } else {
              avgDensity += dRestDensity;
            }
          #endif

          // Calculate new pressure
          float pressureNew = dRestDensity - p.densityIntermediate() - aNeighbors;
          pressureNew *= omega / p.aSelf();
          pressureNew += (1.f - omega) * p.pressure();
          checkValidNumber(pressureNew);
          p.setPressure(pressureNew);
    #if MFluidSolver_USE_TBB
        }
        return partialDensitySum;
      },
      std::plus<float>()
    );
    #else
        }
    #endif

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
