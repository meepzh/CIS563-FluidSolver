//  Copyright 2016 Robert Zhou
//
//  transform.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_GEOM_TRANSFORM_HPP_
#define MFLUIDSOLVER_GEOM_TRANSFORM_HPP_

#include <glm/glm.hpp>

class Transform {
 public:
  Transform() : Transform(glm::vec3(0), glm::vec3(0), glm::vec3(0)) {}
  Transform(const glm::vec3 &translation,
            const glm::vec3 &rotation,
            const glm::vec3 &scale);

  void setTransform(const glm::vec3 &translation,
                    const glm::vec3 &rotation,
                    const glm::vec3 &scale);
  void setTranslation(const glm::vec3 &translation);
  void setRotation(const glm::vec3 &rotation);
  void setScale(const glm::vec3 &scale);

  const glm::mat4 &T() const;
  const glm::mat4 &invT() const;
  const glm::vec3 &position() const;
  const glm::vec3 &rotation() const;
  const glm::vec3 &scale() const;

 private:
  void calculateMatrices();

  glm::vec3 _translation;
  glm::vec3 _rotation;
  glm::vec3 _scale;

  glm::mat4 worldTransform;
  glm::mat4 invWorldTransform;
};

#endif  // MFLUIDSOLVER_GEOM_TRANSFORM_HPP_
