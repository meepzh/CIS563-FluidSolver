//
//  neighborSearch.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_
#define MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_

#include <vector>
#include "../particle.hpp"

enum NeighborSearchType {Naive, StandardGrid};

class NeighborSearch {
public:
  virtual void findNeighbors(Particle *p, std::vector<Particle *> &neighbors) = 0;
  virtual void addParticle(Particle *p) = 0;
};

class NaiveNeighborSearch : public NeighborSearch {
public:
  virtual void findNeighbors(Particle *p, std::vector<Particle *> &neighbors);
  virtual void addParticle(Particle *p);

private:
  std::vector<Particle *> particleList;
};

class StandardGridNeighborSearch : public NeighborSearch {
public:
  StandardGridNeighborSearch(const glm::vec3 &gridMin, const glm::vec3 &gridMax);
  virtual void findNeighbors(Particle *p, std::vector<Particle *> &neighbors);
  virtual void addParticle(Particle *p);

private:
};

#endif /* MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_ */