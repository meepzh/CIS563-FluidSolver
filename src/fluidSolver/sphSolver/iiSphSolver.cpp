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

#if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
#include <limits>
#endif

#include <json/json.h>

#include "utils.hpp"

IISPHSolver::IISPHSolver()
    : diverging(false),
      densityTolerance(MFluidSolver_DEFAULT_DENSITY_TOLERANCE),
      maxIterations(MFluidSolver_DEFAULT_MAX_PRESSURE_SOLVES) {
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
    "maxPressureSolveIterations",
    MFluidSolver_DEFAULT_MAX_PRESSURE_SOLVES).asInt();
  densityTolerance = root["sph"].get(
    "densityTolerance",
    MFluidSolver_DEFAULT_MAX_PRESSURE_SOLVES).asInt();

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: - Max Pressure Solve Iterations: " <<
    maxIterations << std::endl;
  std::cout << "INFO: - Density Tolerance: " <<
    densityTolerance << std::endl;
  #endif
}

virtual void IISPHSolver::calculateParticleMass(float particleSeparation) {
  // Calculate expected density
  float densitySum = kernelFunctions.computePoly6(glm::vec3(0));
  glm::vec3 neighborPos(0);
  for (neighborPos.x = -1 * kernelRadius;
      neighborPos.x <= kernelRadius; neighborPos.x += particleSeparation) {
    for (neighborPos.y = -1 * kernelRadius;
        neighborPos.y <= kernelRadius; neighborPos.y += particleSeparation) {
      for (neighborPos.z = -1 * kernelRadius;
          neighborPos.z <= kernelRadius; neighborPos.z += particleSeparation) {
        densitySum += kernelFunctions.computePoly6(glm::vec3(0) - neighborPos);
      }
    }
  }

  if (calculateMass) {
    mMass = dInitialDensity / densitySum;
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
    std::cout << "INFO: Readjusted particle mass to " << mMass << std::endl;
    #endif
  } else {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
    std::cout << "INFO: Expected initial density is " <<
      (mMass * densitySum) << std::endl;
    #endif
  }
}

void IISPHSolver::update(double deltaT) {
  if (checkIfEnded()) return;

  deltaT = _fixedTimestep;
  const float deltaTF = static_cast<float>(deltaT);
  const float deltaTF2 = static_cast<float>(deltaT * deltaT);

  logTimestep();
  prepNeighborSearch();
  runNeighborSearch();

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
  lastDensityDifference = std::numeric_limits<float>::infinity();
  #endif

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

  // Procedure: Predict Advection
  iter_all_sphparticles_start
    calculateDensity(&p);
    calculateNonPressureForce(&p);

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

  // Procedure: Pressure Solve
  unsigned int iteration = 0;
  unsigned int validParticles;
  float averageDensity = 0;
  const float tolerance = densityTolerance * kernelRadius;
  std::vector<float> nextPressures(_particles.size());
  while (((averageDensity - dRestDensity) > tolerance || iteration < 2)
      && iteration < maxIterations) {
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

    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_WARN
    if (iteration % 50 == 0) {
      #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_DEBUG
      std::cout << "DEBUG: On pressure solve iteration " << iteration <<
        " with density difference " << (averageDensity - dRestDensity) << std::endl;
      #endif

      if (lastDensityDifference < averageDensity - dRestDensity && lastDensityDifference > 1.f) {
        std::cout << "WARN: Density difference is rising! Explosion imminent." << std::endl;
        diverging = true;
        _shouldPauseSimulation = true;
      } else {
        diverging = false;
        _shouldPauseSimulation = false;
      }
      lastDensityDifference = averageDensity - dRestDensity;
    }
    #endif
  }  // end while

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_DEBUG
  std::cout << "DEBUG: Iterated pressure solve " <<
                iteration << " times" << std::endl;
  std::cout << "DEBUG: Density error: " <<
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

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_DEBUG
  std::cout << "DEBUG: Finished one timestep" << std::endl;
  #endif
}
