//
//  particleShaderProgram.cpp
//  MFluidSolver

#include "particleShaderProgram.hpp"

ParticleShaderProgram::ParticleShaderProgram(FluidSolver *solver,
  const std::string &vertexShader, const std::string &fragmentShader,
  const std::string &billboardDDS)
 : ShaderProgram(vertexShader, fragmentShader), solver(solver),
   aBillboardVertexArrID(-1), billboardVertexArrBufferID(-1),
   uCameraRightVecID(-1), uCameraUpVecID(-1) {
  // Get vertex buffer IDs
  aBillboardVertexArrID = glGetAttribLocation(programID, "avs_Billboard");

  // Get uniform IDs
  uCameraRightVecID = glGetUniformLocation(programID, "u_CameraRight");
  uCameraUpVecID = glGetUniformLocation(programID, "u_CameraUp");
  uBillboardTextureSamplerID = glGetUniformLocation(programID, "u_BillboardTextureSampler");

  // Load billboard texture
  billboardTextureID = ShaderProgram::loadDDS(billboardDDS);

  // Fill billboard vertex buffer
  glm::vec3 billboardData[] = {
    glm::vec3(-0.5f, -0.5f, 0),
    glm::vec3(0.5f, -0.5f, 0),
    glm::vec3(-0.5f, 0.5f, 0),
    glm::vec3(0.5f, 0.5f, 0)
  };
  glGenBuffers(1, &billboardVertexArrBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, billboardVertexArrBufferID);
  glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), billboardData, GL_STATIC_DRAW);

  // Pre-allocate particle buffers
  glGenBuffers(1, &particleColorArrBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, particleColorArrBufferID);
  glBufferData(GL_ARRAY_BUFFER, solver->maxParticles * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);

  glGenBuffers(1, &particlePositionArrBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, particlePositionArrBufferID);
  glBufferData(GL_ARRAY_BUFFER, solver->maxParticles * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);

  // Allocate particle space on RAM
  particleColorArray = new glm::vec3[solver->maxParticles];
  particlePositionArray = new glm::vec3[solver->maxParticles];
}

ParticleShaderProgram::~ParticleShaderProgram() {
  if (billboardVertexArrBufferID != -1) glDeleteBuffers(1, &billboardVertexArrBufferID);
  if (particleColorArrBufferID != -1) glDeleteBuffers(1, &particleColorArrBufferID);
  if (particlePositionArrBufferID != -1) glDeleteBuffers(1, &particlePositionArrBufferID);

  delete particleColorArray;
  delete particlePositionArray;
}

void ParticleShaderProgram::setCameraVectors(const glm::vec3 &right, const glm::vec3 &up) {
  glUseProgram(programID);

  if (uCameraRightVecID != -1) {
    // http://glm.g-truc.net/0.9.2/api/a00001.html
    glUniform3f(uCameraRightVecID, right.x, right.y, right.z);
  }

  if (uCameraUpVecID != -1) {
    // http://glm.g-truc.net/0.9.2/api/a00001.html
    glUniform3f(uCameraUpVecID, up.x, up.y, up.z);
  }
}

void ParticleShaderProgram::draw() {
  glUseProgram(programID);

  // Copy data from particles to RAM buffers
  auto particles = solver->particles();
  for (unsigned int i = 0; i < particles->size(); ++i) {
    particleColorArray[i] = particles->at(i)->color;
    particlePositionArray[i] = particles->at(i)->position();
  }

  // Set billboard texture to texture unit 0
  if (billboardTextureID != -1) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, billboardTextureID);
    glUniform1i(billboardTextureID, 0);
  }

  // Update buffers
  if (particleColorArrBufferID != -1) {
    glBindBuffer(GL_ARRAY_BUFFER, particleColorArrBufferID);
    glBufferData(GL_ARRAY_BUFFER, solver->maxParticles * sizeof(glm::vec3), NULL, GL_STREAM_DRAW); // Buffer orphaning for performance
    glBufferSubData(GL_ARRAY_BUFFER, 0, solver->numParticles() * sizeof(glm::vec3), particleColorArray);
  }
  if (particlePositionArrBufferID != -1) {
    glBindBuffer(GL_ARRAY_BUFFER, particlePositionArrBufferID);
    glBufferData(GL_ARRAY_BUFFER, solver->maxParticles * sizeof(glm::vec3), NULL, GL_STREAM_DRAW); // Buffer orphaning for performance
    glBufferSubData(GL_ARRAY_BUFFER, 0, solver->numParticles() * sizeof(glm::vec3), particlePositionArray);
  }

  // Insert data to attribute variables
  if (aBillboardVertexArrID != -1 && billboardVertexArrBufferID != -1) {
    glEnableVertexAttribArray(aBillboardVertexArrID);
    glBindBuffer(GL_ARRAY_BUFFER, billboardVertexArrBufferID);
    glVertexAttribPointer(aBillboardVertexArrID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  }
  if (aVertexColorArrID != -1 && particleColorArrBufferID != -1) {
    glEnableVertexAttribArray(aVertexColorArrID);
    glBindBuffer(GL_ARRAY_BUFFER, particleColorArrBufferID);
    glVertexAttribPointer(aVertexColorArrID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  }
  if (aVertexPositionArrID != -1 && particlePositionArrBufferID != -1) {
    glEnableVertexAttribArray(aVertexPositionArrID);
    glBindBuffer(GL_ARRAY_BUFFER, particlePositionArrBufferID);
    glVertexAttribPointer(aVertexPositionArrID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  }

  // Divide attributes for instancing
  glVertexAttribDivisor(aBillboardVertexArrID, 0);
  glVertexAttribDivisor(aVertexColorArrID, 1);
  glVertexAttribDivisor(aVertexPositionArrID, 1);

  // Draw billboards on all them particles!
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, solver->numParticles());

  // Disable attributes
  if (aBillboardVertexArrID != -1) glDisableVertexAttribArray(aBillboardVertexArrID);
  if (aVertexColorArrID != -1) glDisableVertexAttribArray(aVertexColorArrID);
  if (aVertexPositionArrID != -1) glDisableVertexAttribArray(aVertexPositionArrID);
}