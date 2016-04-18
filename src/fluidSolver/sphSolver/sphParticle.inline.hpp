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

inline glm::vec3 SPHParticle::force() const {
  return _nonPressureForce + _pressureForce;
}

inline glm::vec3 SPHParticle::nonPressureForce() const {
  return _nonPressureForce;
}

inline glm::vec3 SPHParticle::oldPosition() const {
  return _oldPosition;
}

inline float SPHParticle::pressure() const {
  return _pressure;
}

inline glm::vec3 SPHParticle::pressureForce() const {
  return _pressureForce;
}

// Setters
inline void SPHParticle::setDensity(float density) {
  _density = density;
}

inline void SPHParticle::setNonPressureForce(const glm::vec3 &force) {
  _nonPressureForce = force;
}

inline void SPHParticle::setPressure(float pressure) {
  _pressure = pressure;
}

inline void SPHParticle::setPressureForce(const glm::vec3 &force) {
  _pressureForce = force;
}

inline void SPHParticle::setVelocity(const glm::vec3 &velocity) {
  _velocity = velocity;
}

// Neighbors
inline std::vector<SPHParticle *> *SPHParticle::neighbors() {
  return &_neighbors;
}

inline void SPHParticle::clearNeighbors() {
  _neighbors.clear();
}

// IISPH Getters
inline float SPHParticle::advectionDiagonal() const {
  return _advectionDiagonal;
}

inline glm::vec3 SPHParticle::advectionDisplacementEstimate() const {
  return _advectionDisplacementEstimate;
}

inline float SPHParticle::densityIntermediate() const {
  return _densityIntermediate;
}

inline glm::vec3 SPHParticle::sumPressureDisplacementFromNeighbors() const {
  return _sumPressureDisplacementFromNeighbors;
}

inline glm::vec3 SPHParticle::velocityIntermediate() const {
  return _velocityIntermediate;
}

// IISPH Setters
inline void SPHParticle::setAdvectionDiagonal(float a) {
  _advectionDiagonal = a;
}

inline void SPHParticle::setAdvectionDisplacementEstimate(const glm::vec3 &d) {
  _advectionDisplacementEstimate = d;
}

inline void SPHParticle::setDensityIntermediate(float d) {
  _densityIntermediate = d;
}

inline void SPHParticle::setSumPressureDisplacementFromNeighbors(const glm::vec3 &d) {
  _sumPressureDisplacementFromNeighbors = d;
}

inline void SPHParticle::setVelocityIntermediate(const glm::vec3 &v) {
  _velocityIntermediate = v;
}