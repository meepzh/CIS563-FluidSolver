//  Copyright 2016 Robert Zhou
//
//  iiSphSolver.cpp
//  MFluidSolver

#include "iiSphSolver.hpp"

#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

#if MFluidSolver_PARTICLE_STATS_FILES
#include <sstream>
#include <string>
#endif

#include <json/json.h>

#include "utils.hpp"

IISPHSolver::IISPHSolver()
    : maxIterations(1e6) {
}

// TODO: Clean this up
void IISPHSolver::loadConfig(const std::string &file) {
  if (checkInited()) return;
  SPHSolver::loadConfig(file);

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

  maxIterations = root["sph"].get(
    "maxPressureSolveIterations", 1e6).asInt();

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: - Max Pressure Solve Iterations: " <<
    maxIterations << std::endl;
  #endif
}

void IISPHSolver::update(double deltaT) {
  if (checkIfEnded()) return;

  // NOTE: TIMESTEP IS OVERWRITTEN HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  deltaT = _fixedTimestep;
  const float deltaTF = static_cast<float>(deltaT);
  const float deltaTF2 = static_cast<float>(deltaT * deltaT);

  logTimestep();
  prepNeighborSearch();
  runNeighborSearch();

  #if MFluidSolver_PARTICLE_STATS
  averagePosition = glm::vec3(0);
  averageVelocityMagnitude = 0;
  averageDensity = 0;
  averagePressure = 0;
  averageNonPressureForceMagnitude = 0;
  averagePressureForceMagnitude = 0;
  averageAdvectionDiagonal = 0;
  averageAdvectionDisplacementMagnitude = 0;
  averageDensityIntermediate = 0;
  averageSumPressureDisplacementFromNeighborsMagnitude = 0;
  averageVelocityIntermediateMagnitude = 0;
  averageNumNeighbors = 0;

  #if MFluidSolver_PARTICLE_STATS_FILES
  // Output file names
  std::string frameString = MUtils::zeroPad(numUpdates, 6);
  std::ostringstream pressureForceMagnitudeSS;
  pressureForceMagnitudeSS << "particlestats/pfm-" << frameString << ".txt";
  std::ostringstream pressureSS;
  pressureSS << "particlestats/psr-" << frameString << ".txt";
  std::ostringstream velocityMagnitudeSS;
  velocityMagnitudeSS << "particlestats/vel-" << frameString << ".txt";

  // Output files
  std::ofstream pressureForceMagnitudeOutFile;
  pressureForceMagnitudeOutFile.open(
    pressureForceMagnitudeSS.str(),
    std::ofstream::out | std::ofstream::trunc);
  std::ofstream pressureOutFile;
  pressureOutFile.open(
    pressureSS.str(),
    std::ofstream::out | std::ofstream::trunc);
  std::ofstream velocityMagnitudeOutFile;
  velocityMagnitudeOutFile.open(
    velocityMagnitudeSS.str(),
    std::ofstream::out | std::ofstream::trunc);

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_ERROR
  if (!pressureForceMagnitudeOutFile.is_open()) {
    std::cerr << "ERROR: Could not open file: " <<
                  pressureForceMagnitudeSS.str() << std::endl;
  }
  if (!pressureOutFile.is_open()) {
    std::cerr << "ERROR: Could not open file: " <<
                  pressureSS.str() << std::endl;
  }
  if (!velocityMagnitudeOutFile.is_open()) {
    std::cerr << "ERROR: Could not open file: " <<
                  velocityMagnitudeSS.str() << std::endl;
  }
  #endif
  #endif
  #endif

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
  numFlyaways = 0;
  #endif

  // Procedure: Predict Advection
  iter_all_sphparticles_start
    calculateDensity(&p);
    calculateNonPressureForce(&p);

    #if MFluidSolver_PARTICLE_STATS
    // Check if flyaway is affecting other particles
    if (p.flyaway && !p.neighbors()->empty()) {
      for (SPHParticle *q : *(p.neighbors())) {
        std::cout << "STAT: Particle ID " << q->ID <<
          " is now neighbors with flyaway ID " << p.ID << std::endl;
      }
    }
    #endif

    // Flyaway particle detection (no neighbors, too much free will)
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN || MFluidSolver_PARTICLE_STATS
    if (p.neighbors()->empty()) {
      #if MFluidSolver_PARTICLE_STATS
      std::lock_guard<std::mutex> guard(statsCoutMutex);
      p.flyaway = true;
      const glm::vec3 position = p.position();
      const glm::vec3 velocity = p.velocity();
      const glm::vec3 nonPressureForce = p.nonPressureForce();
      const glm::vec3 pressureForce = p.pressureForce();
      const glm::vec3 advectionDisplacementEstimate =
        p.advectionDisplacementEstimate();
      const glm::vec3 sumPressureDisplacementFromNeighbors =
        p.sumPressureDisplacementFromNeighbors();
      const glm::vec3 velocityIntermediate = p.velocityIntermediate();

      std::cout << "WARN: Flyaway particle ID " <<
                          p.ID << " detected! Printing information:" <<
                          std::endl
                << "WARN: - Position: (" <<
                            position.x << ", " <<
                            position.y << ", " <<
                            position.z << ") with magnitude " <<
                            glm::length(position) << std::endl
                << "WARN: - Velocity: (" <<
                            velocity.x << ", " <<
                            velocity.y << ", " <<
                            velocity.z << ") with magnitude " <<
                            glm::length(velocity) << std::endl
                << "WARN: - Density: " << p.density() << std::endl
                << "WARN: - Pressure: " << p.pressure() << std::endl
                << "WARN: - Non-pressure Force: (" <<
                            nonPressureForce.x << ", " <<
                            nonPressureForce.y << ", " <<
                            nonPressureForce.z << ") with magnitude " <<
                            glm::length(nonPressureForce) << std::endl
                << "WARN: - Pressure Force: (" <<
                            pressureForce.x << ", " <<
                            pressureForce.y << ", " <<
                            pressureForce.z << ") with magnitude " <<
                            glm::length(pressureForce) << std::endl
                << "WARN: - Advection Diagonal: " <<
                            p.advectionDiagonal() << std::endl
                << "WARN: - Advection Displacement Estimate: (" <<
                            advectionDisplacementEstimate.x << ", " <<
                            advectionDisplacementEstimate.y << ", " <<
                            advectionDisplacementEstimate.z <<
                            ") with magnitude " <<
                            glm::length(advectionDisplacementEstimate) <<
                            std::endl
                << "WARN: - Density Intermediate: " <<
                            p.densityIntermediate() << std::endl
                << "WARN: - Sum Pressure Displacement From Neighbors: (" <<
                            sumPressureDisplacementFromNeighbors.x << ", " <<
                            sumPressureDisplacementFromNeighbors.y << ", " <<
                            sumPressureDisplacementFromNeighbors.z <<
                            ") with magnitude " <<
                            glm::length(sumPressureDisplacementFromNeighbors) <<
                            std::endl
                << "WARN: - Velocity Intermediate: (" <<
                            velocityIntermediate.x << ", " <<
                            velocityIntermediate.y << ", " <<
                            velocityIntermediate.z << ") with magnitude " <<
                            glm::length(velocityIntermediate) << std::endl;
      #endif
      ++numFlyaways;
    }
    #endif

    // Particle statistics (TODO: Save to file for complex statistics)
    #if MFluidSolver_PARTICLE_STATS
    averagePosition += p.position();
    averageVelocityMagnitude += glm::length(p.velocity());
    averageDensity += p.density();
    averagePressure += p.pressure();
    averageNonPressureForceMagnitude += glm::length(p.nonPressureForce());
    averagePressureForceMagnitude += glm::length(p.pressureForce());
    averageAdvectionDiagonal += p.advectionDiagonal();
    averageAdvectionDisplacementMagnitude +=
      glm::length(p.advectionDisplacementEstimate());
    averageDensityIntermediate += p.densityIntermediate();
    averageSumPressureDisplacementFromNeighborsMagnitude +=
      glm::length(p.sumPressureDisplacementFromNeighbors());
    averageVelocityIntermediateMagnitude +=
      glm::length(p.velocityIntermediate());
    averageNumNeighbors += p.neighbors()->size();

    #if MFluidSolver_PARTICLE_STATS_FILES
    {
      std::lock_guard<std::mutex> guard(statsCoutMutex);
      if (pressureForceMagnitudeOutFile.is_open()) {
          pressureForceMagnitudeOutFile <<
            glm::length(p.pressureForce()) << std::endl;
      }
      if (pressureOutFile.is_open()) {
        pressureOutFile << p.pressure() << std::endl;
      }
      if (velocityMagnitudeOutFile.is_open()) {
        velocityMagnitudeOutFile << glm::length(p.velocity()) << std::endl;
      }
    }
    #endif
    #endif

    // Calculate intermediate velocity v^{adv}_i
    p.setVelocityIntermediate(
      p.velocity() + p.nonPressureForce() / p.mass() * deltaTF);

    // Calculate advection displacement estimate d_{ii}
    glm::vec3 advectionDisplacementEstimate(0);
    const float pDensity2 = p.density() * p.density();
    for (SPHParticle *n : *(p.neighbors())) {
      advectionDisplacementEstimate -= n->mass() / pDensity2 *
        kernelFunctions.computeSpikyGradient(p.position() - n->position());
    }
    p.setAdvectionDisplacementEstimate(
      advectionDisplacementEstimate * deltaTF2);
  iter_all_sphparticles_end

  iter_all_sphparticles_start
    // Set intermediate density and advection diagonal a_{ii}
    float advectionDiagonal = 0;
    float advectionDensity = 0;
    for (SPHParticle *n : *(p.neighbors())) {
      // Calculate spikyGradientIJ
      const glm::vec3 spikyGradientFromNeighbor =
        kernelFunctions.computeSpikyGradient(p.position() - n->position());

      // Calculate dJI
      glm::vec3 displacementToNeighbor =
        -deltaTF2 * p.mass() / (p.density() * p.density()) *
        kernelFunctions.computeSpikyGradient(n->position() - p.position());

      advectionDensity += n->mass() *
        glm::dot(
          p.velocityIntermediate() - n->velocityIntermediate(),
          spikyGradientFromNeighbor);

      advectionDiagonal += n->mass() *
        glm::dot(
          p.advectionDisplacementEstimate() - displacementToNeighbor,
          spikyGradientFromNeighbor);
    }
    p.setDensityIntermediate(p.density() + advectionDensity * deltaTF);
    p.setAdvectionDiagonal(advectionDiagonal);

    // Set iteration=0 pressure p^0_i
    p.setPressure(0.5f * p.pressure());
  iter_all_sphparticles_end

  // Print average statistics
  #if MFluidSolver_PARTICLE_STATS
  const unsigned int _numParticles = _particles.size();
  averagePosition /= _numParticles;
  averageVelocityMagnitude /= _numParticles;
  averageDensity /= _numParticles;
  averagePressure /= _numParticles;
  averageNonPressureForceMagnitude /= _numParticles;
  averagePressureForceMagnitude /= _numParticles;
  averageAdvectionDiagonal /= _numParticles;
  averageAdvectionDisplacementMagnitude /= _numParticles;
  averageDensityIntermediate /= _numParticles;
  averageSumPressureDisplacementFromNeighborsMagnitude /= _numParticles;
  averageVelocityIntermediateMagnitude /= _numParticles;
  averageNumNeighbors /= _numParticles;
  std::cout << "STAT: Printing particle statistics for frame " <<
                      numUpdates << std::endl
            << "STAT: Average position: (" <<
                      averagePosition.x << ", " <<
                      averagePosition.y << ", " <<
                      averagePosition.z << ")" << std::endl
            << "STAT: Average velocity magnitude: " <<
                      averageVelocityMagnitude << std::endl
            << "STAT: Average density: " <<
                      averageDensity << std::endl
            << "STAT: Average pressure: " <<
                      averagePressure << std::endl
            << "STAT: Average non-pressure force magnitude: " <<
                      averageNonPressureForceMagnitude << std::endl
            << "STAT: Average pressure force magnitude: " <<
                      averagePressureForceMagnitude << std::endl
            << "STAT: Average advection diagonal: " <<
                      averageAdvectionDiagonal << std::endl
            << "STAT: Average advection displacement magnitude: " <<
                      averageAdvectionDisplacementMagnitude << std::endl
            << "STAT: Average density intermediate: " <<
                      averageDensityIntermediate << std::endl
            << "STAT: Average sum pressure displacement from neighbors magnitude: " <<
                      averageSumPressureDisplacementFromNeighborsMagnitude << std::endl
            << "STAT: Average velocity intermediate magnitude: " <<
                      averageVelocityIntermediateMagnitude << std::endl
            << "STAT: Average number of neighbors: " <<
                      averageNumNeighbors << std::endl;
  #endif
  // Print number of flyaways (use WARN channel if enabled)
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
  // WARN, >0, any STAT
  if (numFlyaways > 0) {
    std::cout << "WARN: We have " << numFlyaways <<
      " flyaway particle(s) that could potentially crash the simulation" <<
      std::endl;
  }
  #if MFluidSolver_PARTICLE_STATS
  else {
    // WARN, =0, STAT
    std::cout << "STAT: We have 0 flyaway particles!" << std::endl;
  }
  #endif
  #elif MFluidSolver_PARTICLE_STATS
  // !WARN, any #, STAT
  std::cout << "STAT: We have " << numFlyaways <<
    " flyaway particle(s)!" << std::endl;
  #endif

  // Procedure: Pressure Solve
  unsigned int iteration = 0;
  unsigned int validParticles;
  float averageDensity = 0;
  const float tolerance = 0.01f * kernelRadius;
  std::vector<float> nextPressures(_particles.size());
  while (((averageDensity - dRestDensity) > tolerance || iteration < 2)
      && iteration < 1e6) {
    averageDensity = 0;
    validParticles = _particles.size();

    iter_all_sphparticles_start
      // Calculate pressure displacement due to neighbors
      glm::vec3 pressureDisplacementFromNeighbors(0);
      for (SPHParticle *n : *(p.neighbors())) {
        pressureDisplacementFromNeighbors -=
          n->mass() * n->pressure() / (n->density() * n->density()) *
          kernelFunctions.computeSpikyGradient(p.position() - n->position());
      }
      p.setSumPressureDisplacementFromNeighbors(
        pressureDisplacementFromNeighbors * deltaTF2);
    iter_all_sphparticles_end

    #if MFluidSolver_USE_TBB
    averageDensity =
      tbb::parallel_reduce(
        tbb::blocked_range<size_t>(0, _particles.size()), 0.f,
      [&](const tbb::blocked_range<size_t> &r, float partialDensitySum) {
        for (unsigned int i = r.begin(); i != r.end(); ++i) {
    #else
        for (unsigned int i = 0; i < _particles.size(); ++i) {
    #endif
          SPHParticle &p = _particles[i];
          if (MUtils::fequal<float>(p.advectionDiagonal(), 0.f, 0.001f)) {
            // We want to disable the pressure force and reset its values
            nextPressures[i] = 0;
            --validParticles;
            continue;
          }

          // Calculate next pressure
          constexpr float omega = 0.5f;
          float densityDifferenceBySelf = 0;
          float densityDifferenceByNeighborsPressure = 0;
          for (SPHParticle *n : *(p.neighbors())) {
            // Calculate spikyGradientIJ
            glm::vec3 spikyGradientFromNeighbor =
              kernelFunctions.computeSpikyGradient(
                p.position() - n->position());

            // Calculate dJI
            glm::vec3 displacementToNeighbor =
              -deltaTF2 * p.mass() / (p.density() * p.density()) *
              kernelFunctions.computeSpikyGradient(
                n->position() - p.position());

            // Calculate density difference components
            densityDifferenceBySelf += n->mass() *
              glm::dot(p.advectionDisplacementEstimate() -
                displacementToNeighbor, spikyGradientFromNeighbor);

            densityDifferenceByNeighborsPressure += n->mass() * glm::dot(
              p.sumPressureDisplacementFromNeighbors() -
              n->advectionDisplacementEstimate() * n->pressure() -
              n->sumPressureDisplacementFromNeighbors() +
              displacementToNeighbor * p.pressure(), spikyGradientFromNeighbor);
          }
          // Sum for average density
          // Note that density depends on old pressure
          const float tempDensityEstimate = p.densityIntermediate() +
            p.pressure() * densityDifferenceBySelf +
            densityDifferenceByNeighborsPressure;
            // TODO: Check if we should do this
            // p.setDensity(tempDensityEstimate);

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
          float newPressure = dRestDensity - p.densityIntermediate() -
                              densityDifferenceByNeighborsPressure;
          newPressure *= omega / p.advectionDiagonal();
          newPressure += (1.f - omega) * p.pressure();
          nextPressures[i] = newPressure;
    #if MFluidSolver_USE_TBB
        }  // end particle for
        return partialDensitySum;
      },  // end lambda
      std::plus<float>()
    );  // end parallel_reduce
    #else
    }  // end particle for
    #endif

    // Update particles with next iteration pressure
    iter_all_sphparticles_start
      p.setPressure(nextPressures[i]);
    iter_all_sphparticles_end

    averageDensity /= static_cast<float>(validParticles);
    ++iteration;
  }  // end while

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_TRACE
  std::cout << "TRACE: Iterated pressure solve " <<
                iteration << " times" << std::endl;
  std::cout << "TRACE: Density error: " <<
                (averageDensity - dRestDensity) << std::endl;
  #endif

  // Procedure: Iteration
  iter_all_sphparticles_start
    calculatePressureForce(&p);
  iter_all_sphparticles_end

  iter_all_sphparticles_start
    // Update
    glm::vec3 newVel = p.velocityIntermediate() +
      p.pressureForce() / p.mass() * deltaTF;
    glm::vec3 newPos = p.position() + newVel * deltaTF;
    p.update(newVel, newPos);

    enforceBounds(&p);
    visualizeParticle(&p);
  iter_all_sphparticles_end
}
