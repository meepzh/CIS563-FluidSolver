//  Copyright 2016 Robert Zhou
//
//  shaderProgram.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_VIEWER_SHADERPROGRAM_HPP_
#define MFLUIDSOLVER_VIEWER_SHADERPROGRAM_HPP_

#include <GL/glew.h>
#include <cstring>
#include <string>
#include <glm/glm.hpp>

#include "MFluidSolverConfig.hpp"

#include "geom/geom.hpp"

class ShaderProgram {
 public:
  ShaderProgram(const std::string &vertexShader,
                const std::string &fragmentShader);
  ~ShaderProgram();
  void draw(Drawable *d);
  void setModelMat(const glm::mat4 &modelMat);
  void setViewProjectionMat(const glm::mat4 &viewProjectionMat);

  static GLuint loadDDS(const std::string &file);

  // Required for derived classes
  void compile();

  GLuint programID;
  GLuint vertexShaderID;
  GLuint fragmentShaderID;
  GLuint aVertexColorArrID;
  GLuint aVertexPositionArrID;
  GLuint uViewProjectionMatID;

  std::string _vertexShader, _fragmentShader;

 private:
  GLuint uModelMatID;
};

#endif  // MFLUIDSOLVER_VIEWER_SHADERPROGRAM_HPP_
