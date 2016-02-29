//
//  particleShaderProgram.cpp
//  MFluidSolver

#include "particleShaderProgram.hpp"

ParticleShaderProgram::ParticleShaderProgram(FluidSolver *solver, const std::string &vertexShader, const std::string &fragmentShader)
 : ShaderProgram(vertexShader, fragmentShader), solver(solver),
   aBillboardVertexArrID(-1), billboardVertexArrBufferID(-1),
   uCameraRightVecID(-1), uCameraUpVecID(-1) {
  // Get vertex buffer IDs
  aBillboardVertexArrID = glGetAttribLocation(programID, "avs_Billboard");

  // Get uniform IDs
  uCameraRightVecID = glGetUniformLocation(programID, "u_CameraRight");
  uCameraUpVecID = glGetUniformLocation(programID, "u_CameraUp");

  // Fill billboard with standard data
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
  glGenBuffers(1, &particlePositionArrBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, particlePositionArrBufferID);
  glBufferData(GL_ARRAY_BUFFER, solver->maxParticles * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);

  glGenBuffers(1, &particleColorArrBufferID);
  glBindBuffer(GL_ARRAY_BUFFER, particleColorArrBufferID);
  glBufferData(GL_ARRAY_BUFFER, solver->maxParticles * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
}