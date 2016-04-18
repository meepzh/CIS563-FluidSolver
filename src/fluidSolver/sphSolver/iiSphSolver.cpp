//
//  iiSphSolver.cpp
//  MFluidSolver

#include "iiSphSolver.hpp"

#include <cassert>
#include <cmath>

#include "utils.hpp"

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

    // Calculate advection displacement estimate d_{ii}
    glm::vec3 advectionDisplacementEstimate(0);
    float pDensity2 = p.density() * p.density();
    for (SPHParticle *n : *(p.neighbors())) {
      advectionDisplacementEstimate -= n->mass() / pDensity2 *
        kernelFunctions.computeSpikyGradient(p.position() - n->position());
    }
    p.setAdvectionDisplacementEstimate(advectionDisplacementEstimate * deltaTF2);
  iter_all_sphparticles_end

  iter_all_sphparticles_start
    // Set intermediate density and advection diagonal a_{ii}
    float advectionDiagonal = 0;
    float advectionDensity = 0;
    for (SPHParticle *n : *(p.neighbors())) {
      // Calculate spikyGradientIJ
      const glm::vec3 spikyGradientFromNeighbor = kernelFunctions.computeSpikyGradient(p.position() - n->position());

      // Calculate dJI
      glm::vec3 displacementToNeighbor = -deltaTF2 * p.mass() / (p.density() * p.density()) *
        kernelFunctions.computeSpikyGradient(n->position() - p.position());

      advectionDensity += n->mass() *
        glm::dot(p.velocityIntermediate() - n->velocityIntermediate(), spikyGradientFromNeighbor);

      advectionDiagonal += n->mass() *
        glm::dot(p.advectionDisplacementEstimate() - displacementToNeighbor, spikyGradientFromNeighbor);
    }
    p.setDensityIntermediate(p.density() + advectionDensity * deltaTF);
    p.setAdvectionDiagonal(advectionDiagonal);

    // Set iteration=0 pressure p^0_i
    p.setPressure(0.5f * p.pressure());
  iter_all_sphparticles_end

  // Procedure: Pressure Solve
  unsigned int iteration = 0;
  float averageDensity = 0;
  const float tolerance = 0.01f * kernelRadius;
  std::vector<float> nextPressures(_particles.size());
  while (((averageDensity - dRestDensity) > tolerance || iteration < 2) && iteration < 50) {
    averageDensity = 0;

    iter_all_sphparticles_start
      // Calculate pressure displacement due to neighbors
      glm::vec3 pressureDisplacementFromNeighbors(0);
      for (SPHParticle *n : *(p.neighbors())) {
        pressureDisplacementFromNeighbors -= n->mass() * n->pressure() / (n->density() * n->density()) *
          kernelFunctions.computeSpikyGradient(p.position() - n->position());
      }
      p.setSumPressureDisplacementFromNeighbors(pressureDisplacementFromNeighbors * deltaTF2);
    iter_all_sphparticles_end

    #if MFluidSolver_USE_TBB
    averageDensity = tbb::parallel_reduce(tbb::blocked_range<size_t>(0, _particles.size()), 0.f,
      [&](const tbb::blocked_range<size_t> &r, float partialDensitySum) {
        for (unsigned int i = r.begin(); i != r.end(); ++i) {
    #else
        for (unsigned int i = 0; i < _particles.size(); ++i) {
    #endif
          SPHParticle &p = _particles[i];
          if (MUtils::fequal<float>(p.advectionDiagonal(), 0.f, 0.001f)) {
            //p.setPressure(0); // TODO: Check what we should do here
            nextPressures[i] = p.pressure();
            continue;
          }

          // Calculate next pressure
          const float omega = 0.5f;
          float densityDifferenceBySelf = 0;
          float densityDifferenceByNeighborsPressure = 0;
          for (SPHParticle *n : *(p.neighbors())) {
            // Calculate spikyGradientIJ
            glm::vec3 spikyGradientFromNeighbor = kernelFunctions.computeSpikyGradient(p.position() - n->position());

            // Calculate dJI
            glm::vec3 displacementToNeighbor = -deltaTF2 * p.mass() / (p.density() * p.density()) *
              kernelFunctions.computeSpikyGradient(n->position() - p.position());

            // Calculate density difference components
            densityDifferenceBySelf += n->mass() *
              glm::dot(p.advectionDisplacementEstimate() - displacementToNeighbor, spikyGradientFromNeighbor);

            densityDifferenceByNeighborsPressure += n->mass() * glm::dot(
              p.sumPressureDisplacementFromNeighbors() -
              n->advectionDisplacementEstimate() * n->pressure() -
              n->sumPressureDisplacementFromNeighbors() +
              displacementToNeighbor * p.pressure(), spikyGradientFromNeighbor);
          }
          // Sum for average density
          // Note that density depends on old pressure
          float tempDensityEstimate = p.densityIntermediate() + p.pressure() * densityDifferenceBySelf + densityDifferenceByNeighborsPressure;
          //p.setDensity(tempDensityEstimate); // TODO: Check if we should do this

          // Update average density
          #if MFluidSolver_USE_TBB
            if (tempDensityEstimate > dRestDensity) {
              partialDensitySum += tempDensityEstimate;
            } else {
              partialDensitySum += dRestDensity;
            }
          #else
            if (tempDensityEstimate > dRestDensity) {
              averageDensity += tempDensityEstimate;
            } else {
              averageDensity += dRestDensity;
            }
          #endif

          // Calculate new pressure
          float newPressure = dRestDensity - p.densityIntermediate() - densityDifferenceByNeighborsPressure;
          newPressure *= omega / p.advectionDiagonal();
          newPressure += (1.f - omega) * p.pressure();
          nextPressures[i] = newPressure;
    #if MFluidSolver_USE_TBB
        } // end particle for
        return partialDensitySum;
      }, // end lambda
      std::plus<float>()
    ); // end parallel_reduce
    #else
    } // end particle for
    #endif

    // Update particles with next iteration pressure
    iter_all_sphparticles_start
      p.setPressure(nextPressures[i]);
    iter_all_sphparticles_end

    averageDensity /= (float)_particles.size();
    ++iteration;
  } // end while

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_TRACE
  std::cout << "TRACE: Iterated pressure solve " << iteration << " times" << std::endl;
  #endif

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
