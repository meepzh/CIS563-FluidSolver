//
//  particleShaderProgram.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_VIEWER_PARTICLESHADERPROGRAM_HPP_
#define MFLUIDSOLVER_VIEWER_PARTICLESHADERPROGRAM_HPP_

#include "MFluidSolverConfig.hpp"

#include <vector>

#include "../fluidSolver/sphSolver/sphSolver.hpp"
#include "shaderProgram.hpp"

class ParticleShaderProgram : public ShaderProgram {
public:
  ParticleShaderProgram(SPHSolver *solver,
    const std::string &vertexShader, const std::string &fragmentShader,
    const std::string &billboardDDS);
  ~ParticleShaderProgram();
  void draw();
  void setCameraVectors(const glm::vec3 &right, const glm::vec3 &up);
  void setParticleSize(float size);

  glm::vec3 *particleColorArray;
  glm::vec3 *particlePositionArray;

private:
  SPHSolver *solver;
  GLuint billboardTextureID;

  // Variable IDs
  GLuint uBillboardTextureSamplerID;
  GLuint aBillboardVertexArrID;
  GLuint uCameraRightVecID;
  GLuint uCameraUpVecID;
  GLuint uParticleSizeFloatID;

  // Buffer IDs
  GLuint billboardVertexArrBufferID;
  GLuint particleColorArrBufferID;
  GLuint particlePositionArrBufferID;
};

#endif /* MFLUIDSOLVER_VIEWER_PARTICLESHADERPROGRAM_HPP_ */