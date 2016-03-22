//
//  neighborSearch.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_
#define MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_

#include <vector>
#include "../particle.hpp"

class NeighborSearch {
public:
  virtual void findNeighbors(Particle *p, std::vector<Particle *> &neighbors) = 0;
};

class NaiveNeighborSearch : public NeighborSearch {
public:
  virtual void findNeighbors(Particle *p, std::vector<Particle *> &neighbors);
};

class StandardGridNeighborSearch : public NeighborSearch {
public:
  virtual void findNeighbors(Particle *p, std::vector<Particle *> &neighbors);
};

#endif /* MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_ */