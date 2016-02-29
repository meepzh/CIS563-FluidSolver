//
//  transform.cpp
//  MFluidSolver

#include "transform.hpp"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(const glm::vec3 &translation, const glm::vec3 &rotation, const glm::vec3 &scale)
 : _translation(translation), _rotation(rotation), _scale(scale) {
  calculateMatrices();
}

void Transform::calculateMatrices() {
  worldTransform = glm::translate(glm::mat4(1.0f), _translation)
                   * glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.x), glm::vec3(1,0,0))
                   * glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.y), glm::vec3(0,1,0))
                   * glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.z), glm::vec3(0,0,1))
                   * glm::scale(glm::mat4(1.0f), _scale);
  invWorldTransform = glm::inverse(worldTransform);
}

void Transform::setTransform(const glm::vec3 &translation, const glm::vec3 &rotation, const glm::vec3 &scale) {
  _translation = translation;
  _rotation = rotation;
  _scale = scale;
  calculateMatrices();
}

void Transform::setTranslation(const glm::vec3 &translation) {
  _translation = translation;
  calculateMatrices();
}
void Transform::setRotation(const glm::vec3 &rotation) {
  _rotation = rotation;
  calculateMatrices();
}
void Transform::setScale(const glm::vec3 &scale) {
  _scale = scale;
  calculateMatrices();
}

const glm::mat4 &Transform::T() const {
  return worldTransform;
}
const glm::mat4 &Transform::invT() const {
  return invWorldTransform;
}
const glm::vec3 &Transform::position() const {
  return _translation;
}
const glm::vec3 &Transform::rotation() const {
  return _rotation;
}
const glm::vec3 &Transform::scale() const {
  return _scale;
}
