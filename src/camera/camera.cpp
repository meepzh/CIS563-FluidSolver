//
//  camera.cpp
//  MFluidSolver

#include "camera.hpp"

Camera::Camera(unsigned int width, unsigned int height,
               const glm::vec3 &eye, const glm::vec3 &ref, const glm::vec3 &worldUp)
 : _width(width), _height(height),
   _eye(eye), _ref(ref), _worldUp(worldUp) {
}