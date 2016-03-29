//
//  neighborSearch.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_
#define MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_

#include "MFluidSolverConfig.hpp"

#include <vector>

#include "sphGrid.hpp"
#include "sphParticle.hpp"

enum NeighborSearchType {Naive, StandardGrid};

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

class StandardGridNeighborSearch : public NeighborSearch {
public:
  StandardGridNeighborSearch(float r, const glm::vec3 &gridMin, const glm::vec3 &gridMax, float cellSize);
  ~StandardGridNeighborSearch();
  virtual void findNeighbors(SPHParticle *p);
  virtual void addParticle(SPHParticle *p);
  void clear();

  #if MFluidSolver_USE_OPENVDB
  void exportVDB();
  #endif

private:
  SPHGrid *grid;
};

#endif /* MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_ */