//  Copyright 2016 Robert Zhou
//
//  geom.cpp
//  MFluidSolver

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

#include "geom.hpp"

Geometry::Geometry()
  : rng(nullptr) {
}

void Geometry::initRNG() {
  // Seed the Mersenne-Twister RNG
  std::array<int, MFluidSolver_MTRNG_SEED_SIZE> seedData;
  std::random_device rd;
  std::generate_n(seedData.data(), seedData.size(), std::ref(rd));
  std::seed_seq seedSeq(std::begin(seedData), std::end(seedData));
  rng = new std::mt19937(seedSeq);

  // Serialize the seed
  std::stringstream seedStream;
  for (auto it = seedData.begin(); it != seedData.end(); ++it) {
    seedStream << std::setfill ('0') << std::setw(sizeof(int) * 2)
      << std::hex << *it;
  }
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: Particle RNG Seed: " << seedStream.str() << std::endl;
  #endif
}

void Geometry::getBoundsByTransformedMinMax(
    const glm::vec3 &min, const glm::vec3 &max,
    glm::vec3 *outMin, glm::vec3 *outMax) const {
  // Transforms min and max vertices,
  // then gets min and max after they're transformed
  std::vector<glm::vec3> points;
  points.push_back(glm::vec3(min.x, min.y, min.z));
  points.push_back(glm::vec3(min.x, min.y, max.z));
  points.push_back(glm::vec3(min.x, max.y, min.z));
  points.push_back(glm::vec3(min.x, max.y, max.z));
  points.push_back(glm::vec3(max.x, min.y, min.z));
  points.push_back(glm::vec3(max.x, min.y, max.z));
  points.push_back(glm::vec3(max.x, max.y, min.z));
  points.push_back(glm::vec3(max.x, max.y, max.z));

  for (size_t i = 0; i < points.size(); ++i) {
    points[i] = glm::vec3(transform.T() * glm::vec4(points[i], 1.0));
  }

  for (size_t i = 0; i < 3; ++i) {
    (*outMin)[i] = std::numeric_limits<float>::infinity();
    (*outMax)[i] = -std::numeric_limits<float>::infinity();
  }

  for (std::vector<glm::vec3>::iterator it = points.begin();
      it != points.end(); ++it) {
    for (int i = 0; i < 3; ++i) {
      if ((*it)[i] < (*outMin)[i]) {
        (*outMin)[i] = (*it)[i];
      }
      if ((*it)[i] > (*outMax)[i]) {
        (*outMax)[i] = (*it)[i];
      }
    }
  }
}
