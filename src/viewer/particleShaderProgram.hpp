//
//  particleShaderProgram.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_VIEWER_PARTICLESHADERPROGRAM_HPP_
#define MFLUIDSOLVER_VIEWER_PARTICLESHADERPROGRAM_HPP_

#include <vector>
#include "shaderProgram.hpp"
#include "../fluidSolver/fluidSolver.hpp"

class ParticleShaderProgram : public ShaderProgram {
public:
  ParticleShaderProgram(FluidSolver *solver, const std::string &vertexShader, const std::string &fragmentShader);
  ~ParticleShaderProgram();
  void draw();
  void setCameraVectors(const glm::vec3 &right, const glm::vec3 &up);

  glm::vec3 *particleColorArray;
  glm::vec3 *particlePositionArray;

private:
  FluidSolver *solver;

  // Variable IDs
  GLuint aBillboardVertexArrID;
  GLuint uCameraRightVecID;
  GLuint uCameraUpVecID;

  // Buffer IDs
  GLuint billboardVertexArrBufferID;
  GLuint particleColorArrBufferID;
  GLuint particlePositionArrBufferID;
};

#endif /* MFLUIDSOLVER_VIEWER_PARTICLESHADERPROGRAM_HPP_ */