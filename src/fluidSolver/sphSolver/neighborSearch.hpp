//
//  neighborSearch.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_
#define MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_

#include <vector>
#include "../particle.hpp"

enum NeighborSearchType {Naive, StandardGrid};

#define DEFAULT_SEARCH_RADIUS 0.1f

class NeighborSearch {
public:
  NeighborSearch() : NeighborSearch(DEFAULT_SEARCH_RADIUS) {}
  NeighborSearch(float r);
  void setSearchRadius(float r);
  virtual void findNeighbors(Particle *p, std::vector<Particle *> &neighbors) = 0;
  virtual void addParticle(Particle *p) = 0;
  virtual void updateParticle(Particle *p) = 0;

protected:
  float searchRadius;
  float searchRadius2;
};

class NaiveNeighborSearch : public NeighborSearch {
public:
  NaiveNeighborSearch() : NaiveNeighborSearch(DEFAULT_SEARCH_RADIUS) {}
  NaiveNeighborSearch(float r) : NeighborSearch(r) {}
  virtual void findNeighbors(Particle *p, std::vector<Particle *> &neighbors);
  virtual void addParticle(Particle *p);
  virtual void updateParticle(Particle *p);

private:
  std::vector<Particle *> particleList;
};

class StandardGridNeighborSearch : public NeighborSearch {
public:
  StandardGridNeighborSearch(float r, const glm::vec3 &gridMin, const glm::vec3 &gridMax);
  virtual void findNeighbors(Particle *p, std::vector<Particle *> &neighbors);
  virtual void addParticle(Particle *p);
  virtual void updateParticle(Particle *p);

private:
};

#endif /* MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_ */