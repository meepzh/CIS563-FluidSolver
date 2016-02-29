//
//  shaderProgram.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_VIEWER_SHADERPROGRAM_HPP_
#define MFLUIDSOLVER_VIEWER_SHADERPROGRAM_HPP_

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../geom/geom.hpp"

class ShaderProgram {
public:
  ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);
  ~ShaderProgram();
  void draw(Drawable *d);
  void setModelMat(const glm::mat4 &modelMat);
  void setViewProjectionMat(const glm::mat4 &viewProjectionMat);

private:
  GLuint programID;

  GLuint vertexShaderID;
  GLuint fragmentShaderID;

  GLuint aVertexColorArrID;
  GLuint aVertexPositionArrID;
  GLuint uModelMatID;
  GLuint uViewProjectionMatID;
};

#endif /* MFLUIDSOLVER_VIEWER_SHADERPROGRAM_HPP_ */