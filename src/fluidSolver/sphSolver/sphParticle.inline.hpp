//
//  sphParticle.inline.hpp
//  MFluidSolver

inline void SPHParticle::update(const glm::vec3 &newVel, const glm::vec3 &newPos) {
  _oldPosition = _position;
  _velocity = newVel;
  _position = newPos;
}

// Getters
inline float SPHParticle::density() const {
  return _density;
}

inline glm::vec3 SPHParticle::forceDensity() const {
  return _nonPressureForceDensity + _pressureForceDensity;
}

inline glm::vec3 SPHParticle::nonPressureForceDensity() const {
  return _nonPressureForceDensity;
}

inline glm::vec3 SPHParticle::oldPosition() const {
  return _oldPosition;
}

inline float SPHParticle::pressure() const {
  return _pressure;
}

inline glm::vec3 SPHParticle::pressureForceDensity() const {
  return _pressureForceDensity;
}

// Setters
inline void SPHParticle::setDensity(float density) {
  _density = density;
}

inline void SPHParticle::setNonPressureForceDensity(const glm::vec3 &forceDensity) {
  _nonPressureForceDensity = forceDensity;
}

inline void SPHParticle::setPressure(float pressure) {
  _pressure = pressure;
}

inline void SPHParticle::setPressureForceDensity(const glm::vec3 &forceDensity) {
  _pressureForceDensity = forceDensity;
}

// Neighbors
inline std::vector<SPHParticle *> *SPHParticle::neighbors() {
  return &_neighbors;
}

inline void SPHParticle::clearNeighbors() {
  _neighbors.clear();
}