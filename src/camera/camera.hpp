//
//  camera.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_CAMERA_HPP_
#define MFLUIDSOLVER_CAMERA_HPP_

#include <glm/glm.hpp>

class Camera {
public:
  Camera() : Camera(1024, 768) {}
  Camera(unsigned int width, unsigned int height)
   : Camera(width, height, glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)) {}
  Camera(unsigned int width, unsigned int height, const glm::vec3 &eye, const glm::vec3 &ref, const glm::vec3 &worldUp);

  // Properties
  float nearClip,
        farClip;

private:
  // Dimensions
  unsigned int _width,
               _height;

  // Orientation vars
  glm::vec3 _eye,
            _ref,
            _look,
            _up,
            _right,
            _worldUp,
            _V,
            _H;

  // Other properties
  float _fovy,
        _aspect;
};

#endif /* MFLUIDSOLVER_CAMERA_HPP_ */
