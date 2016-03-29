//
//  neighborSearch.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_
#define MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_

#include "MFluidSolverConfig.hpp"

#include <vector>

#include "sphUniformGrid.hpp"
#include "sphParticle.hpp"

enum NeighborSearchType {Naive, UniformGrid};

class NeighborSearch {
public:
  NeighborSearch() : NeighborSearch(MFluidSolver_DEFAULT_SEARCH_RADIUS) {}
  NeighborSearch(float r);
  void setSearchRadius(float r);
  virtual void findNeighbors(SPHParticle *p) = 0;
  virtual void addParticle(SPHParticle *p) = 0;

protected:
  float searchRadius;
  float searchRadius2;
};

class NaiveNeighborSearch : public NeighborSearch {
public:
  NaiveNeighborSearch() : NaiveNeighborSearch(MFluidSolver_DEFAULT_SEARCH_RADIUS) {}
  NaiveNeighborSearch(float r) : NeighborSearch(r) {}
  virtual void findNeighbors(SPHParticle *p);
  virtual void addParticle(SPHParticle *p);

private:
  std::vector<SPHParticle *> particleList;
};

class UniformGridNeighborSearch : public NeighborSearch {
public:
  UniformGridNeighborSearch(float r, const glm::vec3 &gridMin, const glm::vec3 &gridMax, float cellSize);
  ~UniformGridNeighborSearch();
  virtual void findNeighbors(SPHParticle *p);
  virtual void addParticle(SPHParticle *p);
  void clear();

  #if MFluidSolver_USE_OPENVDB
  void exportVDB();
  #endif

private:
  SPHUniformGrid *grid;
};

#endif /* MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_ */