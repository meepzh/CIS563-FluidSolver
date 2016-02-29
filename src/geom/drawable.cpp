//
//  drawable.cpp
//  MFluidSolver

#include "drawable.hpp"

Drawable::Drawable() : vertexIndexArrBufferID(-1), vertexPositionArrBufferID(-1) {
}

Drawable::~Drawable() {
  if (vertexIndexArrBufferID != -1) glDeleteBuffers(1, &vertexIndexArrBufferID);
  if (vertexPositionArrBufferID != -1) glDeleteBuffers(1, &vertexPositionArrBufferID);
}

bool Drawable::bindIndexBuffer() {
  if (vertexIndexArrBufferID == -1) return false;
  glBindBuffer(GL_ARRAY_BUFFER, vertexIndexArrBufferID);
  return true;
}

bool Drawable::bindColorBuffer() {
  if (vertexColorArrBufferID == -1) return false;
  glBindBuffer(GL_ARRAY_BUFFER, vertexColorArrBufferID);
  return true;
}

bool Drawable::bindPositionBuffer() {
  if (vertexPositionArrBufferID == -1) return false;
  glBindBuffer(GL_ARRAY_BUFFER, vertexPositionArrBufferID);
  return true;
}

GLenum Drawable::drawMode() {
  return GL_TRIANGLES;
}

GLenum Drawable::idxCount() {
  return _idxCount;
}