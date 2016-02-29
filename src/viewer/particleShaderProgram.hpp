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
  void draw(std::vector<Particle *> &particles);

private:
  FluidSolver *solver;

  // Variable IDs
  GLuint aBillboardVertexArrID;
  GLuint uCameraRightVecID;
  GLuint uCameraUpVecID;

  // Buffer IDs
  GLuint billboardVertexArrBufferID;
  GLuint particlePositionArrBufferID;
  GLuint particleColorArrBufferID;
};

#endif /* MFLUIDSOLVER_VIEWER_PARTICLESHADERPROGRAM_HPP_ */