//  Copyright 2016 Robert Zhou
//
//  drawable.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_GEOM_DRAWABLE_HPP_
#define MFLUIDSOLVER_GEOM_DRAWABLE_HPP_

#include <GL/glew.h>

class Drawable {
 public:
  Drawable();
  ~Drawable();
  bool bindIndexBuffer();
  bool bindColorBuffer();
  bool bindPositionBuffer();
  virtual GLenum drawMode();
  GLenum idxCount();

 protected:
  GLuint vertexIndexArrBufferID;
  GLuint vertexColorArrBufferID;
  GLuint vertexPositionArrBufferID;
  int _idxCount;
};

#endif  // MFLUIDSOLVER_GEOM_DRAWABLE_HPP_
