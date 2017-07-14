//  Copyright 2016 Robert Zhou
//
//  cube.cpp
//  MFluidSolver

#include "cube.hpp"

#include <iostream>
#include <random>

#define GLM_FORCE_RADIANS
#include <glm/gtx/norm.hpp>

Cube::Cube(const glm::vec3 &color) : _color(color) {
  const unsigned int IDX_COUNT = 24;
  const unsigned int VERT_COUNT = 8;

  // Vertices
  glm::vec3 vert_pos[VERT_COUNT];
  vert_pos[0] = glm::vec3(0.5f, 0.5f, 0.5f);
  vert_pos[1] = glm::vec3(-0.5f, 0.5f, 0.5f);
  vert_pos[2] = glm::vec3(0.5f, -0.5f, 0.5f);
  vert_pos[3] = glm::vec3(-0.5f, -0.5f, 0.5f);
  vert_pos[4] = glm::vec3(0.5f, 0.5f, -0.5f);
  vert_pos[5] = glm::vec3(-0.5f, 0.5f, -0.5f);
  vert_pos[6] = glm::vec3(0.5f, -0.5f, -0.5f);
  vert_pos[7] = glm::vec3(-0.5f, -0.5f, -0.5f);

  // Indices
  GLuint idx[IDX_COUNT];
  idx[0] = 0; idx[1] = 1;
  idx[2] = 0; idx[3] = 2;
  idx[4] = 0; idx[5] = 4;
  idx[6] = 1; idx[7] = 3;
  idx[8] = 1; idx[9] = 5;
  idx[10] = 2; idx[11] = 3;
  idx[12] = 2; idx[13] = 6;
  idx[14] = 3; idx[15] = 7;
  idx[16] = 4; idx[17] = 5;
  idx[18] = 4; idx[19] = 6;
  idx[20] = 5; idx[21] = 7;
  idx[22] = 6; idx[23] = 7;

  // Color
  glm::vec3 vert_col[VERT_COUNT];
  for (unsigned int i = 0; i < VERT_COUNT; ++i) {
    vert_col[i] = _color;
  }

  _idxCount = IDX_COUNT;

  // Bind
  glGenBuffers(1, &vertexIndexArrBufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexArrBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, IDX_COUNT * sizeof(GLuint), idx,
    GL_STATIC_DRAW);

  glGenBuffers(1, &vertexPositionArrBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, vertexPositionArrBufferID);
  glBufferData(GL_ARRAY_BUFFER, VERT_COUNT * sizeof(glm::vec3), vert_pos,
    GL_STATIC_DRAW);

  glGenBuffers(1, &vertexColorArrBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, vertexColorArrBufferID);
  glBufferData(GL_ARRAY_BUFFER, VERT_COUNT * sizeof(glm::vec3), vert_col,
    GL_STATIC_DRAW);
}

GLenum Cube::drawMode() {
  return GL_LINES;
}

bool Cube::intersects(const glm::vec3 &point) const {
  glm::vec3 localPos = glm::vec3(transform.invT() * glm::vec4(point, 1.f));
  if (localPos.x >= -0.5f && localPos.x <= 0.5f &&
      localPos.y >= -0.5f && localPos.y <= 0.5f &&
      localPos.z >= -0.5f && localPos.z <= 0.5f)
    return true;
  return false;
}

void Cube::spawnParticlesInVolume(
    FluidSolver *solver, ParticleSpawnMethod spawnMethod) {
  // Store old particle count for number of particles we added and max particle check
  unsigned int oldParticleCount = solver->numParticles();
  if (oldParticleCount == solver->maxParticles()) {
    #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
    std::cout << "INFO: Reached max number of particles (" <<
      solver->maxParticles() << ")!" << std::endl;
    #endif
    return;
  }

  unsigned int particlesLeft = solver->maxParticles() - oldParticleCount;
  glm::vec3 minBound, maxBound;
  getBoundsByTransformedMinMax(
    glm::vec3(-0.5f), glm::vec3(0.5f), &minBound, &maxBound);

  float particleSeparation = solver->particleSeparation();
  std::uniform_real_distribution<float> unifDist(0, particleSeparation);
  unsigned int count = 0;

  // Calculate padding due to particle separation
  glm::vec3 padding = maxBound - minBound;
  glm::ivec3 numParticlesOnAxis = (glm::ivec3) (padding / particleSeparation);
  padding -= (glm::vec3)numParticlesOnAxis * particleSeparation;
  minBound += padding / 2.0f;

  // Create particles
  if (spawnMethod == ParticleSpawnMethod::Uniform) {
    glm::vec3 spawnPos(0);
    // X-loop
    for (unsigned int iIdx = 0; ; ++iIdx) {
      spawnPos.x = minBound.x + iIdx * particleSeparation;
      if (spawnPos.x > maxBound.x) break;
      // Y-loop
      for (unsigned int jIdx = 0; ; ++jIdx) {
        spawnPos.y = minBound.y + jIdx * particleSeparation;
        if (spawnPos.y > maxBound.y) break;
        // Z-loop
        for (unsigned int kIdx = 0; ; ++kIdx) {
          spawnPos.z = minBound.z + kIdx * particleSeparation;
          if (spawnPos.z > maxBound.z) break;
          // Loop body
          solver->addParticleAt(spawnPos);
          ++count;
          // Loop exit
          if (count > particlesLeft) break;
        }
        if (count > particlesLeft) break;
      }
      if (count > particlesLeft) break;
    }
  } else if (spawnMethod == ParticleSpawnMethod::Jittered) {
    initRNG();
    for (float i = minBound.x; i <= maxBound.x - particleSeparation; i += particleSeparation) {
      for (float j = minBound.y; j <= maxBound.y - particleSeparation; j += particleSeparation) {
        for (float k = minBound.z; k <= maxBound.z - particleSeparation; k += particleSeparation) {
          const float xOffset = unifDist(*rng);
          const float yOffset = unifDist(*rng);
          const float zOffset = unifDist(*rng);
          const float factor = 0.27f;
          solver->addParticleAt(glm::vec3(
            i + xOffset * factor, j + yOffset * factor, k + zOffset * factor));
          ++count;
          if (count > particlesLeft) break;
        }
        if (count > particlesLeft) break;
      }
      if (count > particlesLeft) break;
    }
  }

  if (solver->numParticles() == solver->maxParticles()) {
    std::cout << "INFO: Reached max number of particles (" <<
      solver->maxParticles() << ")!" << std::endl;
  }

  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: Cube (" << name << ") seeded " <<
    (solver->numParticles() - oldParticleCount) << " particles" << std::endl;
  #endif
}

bool Cube::intersects(const glm::vec3 &point, glm::ivec3 *violations) const {
  glm::vec3 localPos = glm::vec3(transform.invT() * glm::vec4(point, 1.f));

  // Check each axis locally
  violations->x = 0;
  if (localPos.x <= -0.5f) violations->x = -1;
  else if (localPos.x > 0.5f) violations->x = 1;

  violations->y = 0;
  if (localPos.y <= -0.5f) violations->y = -1;
  else if (localPos.y > 0.5f) violations->y = 1;

  violations->z = 0;
  if (localPos.z <= -0.5f) violations->z = -1;
  else if (localPos.z > 0.5f) violations->z = 1;

  return violations->x == 0 && violations->y == 0 && violations->z == 0;
}
