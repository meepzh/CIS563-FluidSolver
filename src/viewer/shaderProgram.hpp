//
//  shaderProgram.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_VIEWER_SHADERPROGRAM_HPP_
#define MFLUIDSOLVER_VIEWER_SHADERPROGRAM_HPP_

#include <string>
#include <GL/glew.h>
#include "../geom/drawable.hpp"

class ShaderProgram {
public:
  ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader);
  void draw(Drawable *d);
  void glDelete();

private:
  GLuint programID;

  GLuint vertexShaderID;
  GLuint fragmentShaderID;

  GLuint aVertexPositionArrID;
  GLuint uColorVecID;
  GLuint uModelMatID;
  GLuint uViewProjectionMatID;
};

#endif /* MFLUIDSOLVER_VIEWER_SHADERPROGRAM_HPP_ */