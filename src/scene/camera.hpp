//
//  camera.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_CAMERA_HPP_
#define MFLUIDSOLVER_CAMERA_HPP_

#include "MFluidSolverConfig.hpp"

#include <glm/glm.hpp>

class Camera {
public:
  Camera() : Camera(1024, 768) {}
  Camera(unsigned int width, unsigned int height)
   : Camera(width, height, glm::vec3(0, 0, 12), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)) {}
  Camera(unsigned int width, unsigned int height, const glm::vec3 &eye, const glm::vec3 &ref, const glm::vec3 &worldUp);

  // Matrices
  glm::mat4 getViewProjection();

  void setEyeRef(const glm::vec3 &eye, const glm::vec3 &ref);
  void recomputeAttributes();
  void recomputeLocalAxes();
  void recomputeEyeAndRef();

  // Arcball
  void arcball(const glm::dvec2 &p1, const glm::dvec2 &p2); // Rotation interface for the mouse
  void pan(const glm::dvec2 &p1, const glm::dvec2 &p2); // Panning interface for the mouse
  void zoom(double delta); // Zooming interface for the scroll wheel

  // Properties
  float nearClip,
        farClip;

  const glm::vec3 right() const;
  const glm::vec3 up() const;

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

  // Arcball
  float centerX,
        centerY,
        arcballRadius,
        panSpeed,
        zoomSpeed;
  glm::mat4 arcballRotationMat;
  glm::vec4 arcballEye,
            arcballRef,
            arcballLook,
            arcballUp,
            arcballRight;
  glm::vec4 arcballPan;
  float arcballZoom;
  glm::vec3 computeSpherePoint(const glm::dvec2 &p) const;

  // Other properties
  float _fovy,
        _aspect;
};

#endif /* MFLUIDSOLVER_CAMERA_HPP_ */
