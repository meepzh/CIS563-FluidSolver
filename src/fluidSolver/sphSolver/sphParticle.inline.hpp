//
//  sphParticle.inline.hpp
//  MFluidSolver

inline void SPHParticle::update(const glm::vec3 &newVel, const glm::vec3 &newPos) {
  _oldPosition = _position;
  _velocity = newVel;
  _position = newPos;
}

inline float SPHParticle::density() const {
  return _density;
}

inline float SPHParticle::pressure() const {
  return _pressure;
}

inline glm::vec3 SPHParticle::forceDensity() const {
  return _forceDensity;
}

inline glm::vec3 SPHParticle::oldPosition() const {
  return _oldPosition;
}

inline void SPHParticle::setDensity(float density) {
  _density = density;
}

inline void SPHParticle::setPressure(float pressure) {
  _pressure = pressure;
}

inline void SPHParticle::setForceDensity(const glm::vec3 &forceDensity) {
  _forceDensity = forceDensity;
}

inline void SPHParticle::addForceDensity(const glm::vec3 &forceDensity) {
  _forceDensity += forceDensity;
}

inline std::vector<SPHParticle *> *SPHParticle::neighbors() {
  return &_neighbors;
}

inline void SPHParticle::clearNeighbors() {
  _neighbors.clear();
}