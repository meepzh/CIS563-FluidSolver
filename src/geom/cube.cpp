//
//  cube.cpp
//  MFluidSolver

#include "cube.hpp"

#define GLM_FORCE_RADIANS
#include <glm/gtx/norm.hpp>

Cube::Cube(const glm::vec3 &color) : _color(color) {
  const unsigned int IDX_COUNT = 24;
  const unsigned int VERT_COUNT = 8;

  // Indices
  GLuint idx[IDX_COUNT];
  /*idx[0] = 0; idx[1] = 1; // Draw 2 squares
  idx[2] = 1; idx[3] = 3;
  idx[4] = 3; idx[5] = 2;
  idx[6] = 2; idx[7] = 0;
  idx[8] = 4; idx[9] = 5;
  idx[10] = 5; idx[11] = 7;
  idx[12] = 7; idx[13] = 6;
  idx[14] = 6; idx[15] = 4;
  for (unsigned int i = 0; i < 4; ++i) {
    // Connect sides
    idx[i * 2 + 16] = i;
    idx[i * 2 + 17] = i + 4;
  }*/

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

  unsigned int counter = 0;
  unsigned int idxCount = 0;
  for (unsigned int i = 0; i < VERT_COUNT; ++i) {
    for (unsigned int j = i + 1; j < VERT_COUNT; ++j) {
      if (idxCount == IDX_COUNT) break;
      if (glm::distance2(vert_pos[i], vert_pos[j]) < 1.0001f && glm::distance2(vert_pos[i], vert_pos[j]) > 0.999f) {
        idx[idxCount++] = i;
        idx[idxCount++] = j;
        counter++;
      }
    }
    if (idxCount == IDX_COUNT) break;
  }

  // Color
  glm::vec3 vert_col[VERT_COUNT];
  for (unsigned int i = 0; i < VERT_COUNT; ++i) {
    vert_col[i] = _color;
  }

  _idxCount = IDX_COUNT;

  // Bind
  glGenBuffers(1, &vertexIndexArrBufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexArrBufferID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, IDX_COUNT * sizeof(GLuint), idx, GL_STATIC_DRAW);

  glGenBuffers(1, &vertexPositionArrBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, vertexPositionArrBufferID);
  glBufferData(GL_ARRAY_BUFFER, VERT_COUNT * sizeof(glm::vec3), vert_pos, GL_STATIC_DRAW);

  glGenBuffers(1, &vertexColorArrBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, vertexColorArrBufferID);
  glBufferData(GL_ARRAY_BUFFER, VERT_COUNT * sizeof(glm::vec3), vert_col, GL_STATIC_DRAW);
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

void Cube::spawnParticlesInVolume(FluidSolver *solver) const {
  glm::vec3 minBound, maxBound;
  getBoundsByTransformedMinMax(glm::vec3(-0.5f), glm::vec3(0.5f), minBound, maxBound);

  double particleSeparation = solver->particleSeparation();
  unsigned int count = 0;
  minBound += particleSeparation / 2.f;
  maxBound -= particleSeparation / 2.f;
  for (double i = minBound.x; i <= maxBound.x; i += particleSeparation) {
    for (double j = minBound.y; j <= maxBound.y; j += particleSeparation) {
      for (double k = minBound.z; k <= maxBound.z; k += particleSeparation) {
        solver->addParticleAt(glm::vec3(i, j, k));
        ++count;
      }
    }
  }
  //solver->addParticle(new Particle(glm::vec3(0)));
  printf("INFO: Seeded %d particles.\n", count);
}

bool Cube::intersects(const glm::vec3 &point, glm::ivec3 &violations) const {
  glm::vec3 localPos = glm::vec3(transform.invT() * glm::vec4(point, 1.f));

  violations.x = 0;
  if (localPos.x <= -0.5f) violations.x = -1;
  else if (localPos.x > 0.5f) violations.x = 1;

  violations.y = 0;
  if (localPos.y <= -0.5f) violations.y = -1;
  else if (localPos.y > 0.5f) violations.y = 1;

  violations.z = 0;
  if (localPos.z <= -0.5f) violations.z = -1;
  else if (localPos.z > 0.5f) violations.z = 1;

  return violations.x == 0 && violations.y == 0 && violations.z == 0;
}