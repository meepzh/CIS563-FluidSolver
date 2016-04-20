//  Copyright 2016 Robert Zhou
//
//  camera.cpp
//  MFluidSolver

#include "camera.hpp"

#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(unsigned int width, unsigned int height,
               const glm::vec3 &eye, const glm::vec3 &ref,
               const glm::vec3 &worldUp)
    : nearClip(0.1f), farClip(1000.f),
      _width(width), _height(height),
      _eye(eye), _ref(ref), _worldUp(worldUp),
      _fovy(45.f), arcballZoom(1.f),
      panSpeed(0.01f), zoomSpeed(0.005f) {
  arcballRotationMat = glm::mat4(1.f);
  recomputeAttributes();
  recomputeLocalAxes();
  recomputeEyeAndRef();
}

glm::mat4 Camera::getViewProjection() {
  glm::mat4 perpMat =
    glm::perspective(glm::radians(_fovy), _aspect, nearClip, farClip);
  glm::mat4 lookMat =
    glm::lookAt(
      glm::vec3(arcballEye), glm::vec3(arcballRef), glm::vec3(arcballUp));
  return perpMat * lookMat;
}

void Camera::setEyeRef(const glm::vec3 &eye, const glm::vec3 &ref) {
  #if MFluidSolver_LOG_LEVEL <= MFluidSolver_LOG_INFO
  std::cout << "INFO: Set camera to eye (" <<
                eye.x << ", " <<
                eye.y << ", " <<
                eye.z << ") and ref (" <<
                ref.x << ", " <<
                ref.y << ", " <<
                ref.z << ")" << std::endl;
  #endif

  _eye = eye;
  _ref = ref;
  arcballZoom = 1.f;
  arcballPan = glm::vec4(0);
  arcballRotationMat = glm::mat4(1.f);

  recomputeAttributes();
  recomputeLocalAxes();
  recomputeEyeAndRef();
}

void Camera::recomputeAttributes() {
  // Vectors
  _look = glm::normalize(_ref - _eye);
  _right = glm::normalize(glm::cross(_look, _worldUp));
  _up = glm::normalize(glm::cross(_right, _look));

  // Other
  _aspect = static_cast<float>(_width) / static_cast<float>(_height);

  // Arcball
  centerX = _width / 2.f;
  centerY = _height / 2.f;
  arcballRadius = _width * 0.30;  // Arbitrary radius
}

void Camera::recomputeLocalAxes() {
  arcballLook = glm::normalize(arcballRotationMat * glm::vec4(_look, 0));
  arcballUp = glm::normalize(arcballRotationMat * glm::vec4(_up, 0));
  arcballRight = glm::normalize(arcballRotationMat * glm::vec4(_right, 0));
}

void Camera::recomputeEyeAndRef() {
  arcballEye = arcballRotationMat * glm::vec4(_eye, 0);
  arcballEye += arcballPan;
  arcballRef = glm::vec4(_ref, 1.f) + arcballPan;

  // Apply zoom
  glm::vec4 difference = arcballEye - arcballRef;
  difference *= arcballZoom;
  arcballEye = arcballRef + difference;
}

void Camera::arcball(const glm::dvec2 &p1, const glm::dvec2 &p2) {
  glm::vec3 pt1 = computeSpherePoint(p1);
  glm::vec3 pt2 = computeSpherePoint(p2);

  glm::vec3 ptCross = glm::cross(pt1, pt2);
  glm::quat dq = glm::quat(glm::dot(pt1, pt2), ptCross.x, ptCross.y, ptCross.z);

  arcballRotationMat *= glm::mat4_cast(dq * glm::quat(1.f, 0, 0, 0));

  recomputeLocalAxes();
  recomputeEyeAndRef();
}

glm::vec3 Camera::computeSpherePoint(const glm::dvec2 &p) const {
  glm::vec3 p2(0);
  p2.x = (p.x - centerX) / arcballRadius;
  p2.y = (p.y - centerY) / arcballRadius;

  float r = (p2.x * p2.x) + (p2.y * p2.y);
  if (r > 1) {
    p2 = glm::normalize(p2);
  } else {
    p2.x *= -1;  // Reverse x position
    p2.z = sqrt(1 - r);
  }

  return p2;
}

void Camera::pan(const glm::dvec2 &p1, const glm::dvec2 &p2) {
  float dx = p2.x - p1.x;
  float dy = p2.y - p1.y;

  arcballPan -= dx * panSpeed * arcballRight;
  arcballPan += dy * panSpeed * arcballUp;

  recomputeEyeAndRef();
}

void Camera::zoom(double delta) {
  // A typical mouse has 15 * 8 delta (eigth-degrees) per scroll wheel tick
  arcballZoom += delta * zoomSpeed;

  // Prevent zooming past ref
  if (arcballZoom < 0) {
    arcballZoom = 0;
  }

  recomputeEyeAndRef();
}

const glm::vec3 Camera::right() const {
  return glm::vec3(arcballRight);
}

const glm::vec3 Camera::up() const {
  return glm::vec3(arcballUp);
}
