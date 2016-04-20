//  Copyright 2016 Robert Zhou
//
//  neighborSearch.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_
#define MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_

#include <vector>

#include "MFluidSolverConfig.hpp"

#include "sphParticle.hpp"
#include "sphUniformGrid.hpp"
#include "sphZIndexSortedUniformGrid.hpp"

enum NeighborSearchType {
  Naive, UniformGrid,
  IndexSortedUniformGrid, IndexSortedUniformGridWithInsertion,
  ZIndexSortedUniformGrid, ZIndexSortedUniformGridWithInsertion};

class NeighborSearch {
 public:
  NeighborSearch() : NeighborSearch(MFluidSolver_DEFAULT_SEARCH_RADIUS) {}
  explicit NeighborSearch(float r);
  void setSearchRadius(float r);
  void printPerformanceStats();
  virtual void findNeighbors(SPHParticle *p) = 0;
  virtual void addParticle(SPHParticle *p) = 0;
  virtual void updateParticle(SPHParticle *p) = 0;
  virtual void clear() = 0;

 protected:
  float searchRadius;
  float searchRadius2;
};

class NaiveNeighborSearch : public NeighborSearch {
 public:
  NaiveNeighborSearch()
    : NaiveNeighborSearch(MFluidSolver_DEFAULT_SEARCH_RADIUS) {}
  explicit NaiveNeighborSearch(float r) : NeighborSearch(r) {}
  virtual void findNeighbors(SPHParticle *p);
  virtual void addParticle(SPHParticle *p);
  virtual void updateParticle(SPHParticle *p) {}  // Do nothing
  virtual void clear();

 private:
  std::vector<SPHParticle *> particleList;
};

class GridNeighborSearch : public NeighborSearch {
 public:
  explicit GridNeighborSearch(float r);
  ~GridNeighborSearch();
  virtual void findNeighbors(SPHParticle *p);
  virtual void addParticle(SPHParticle *p);
  virtual void updateParticle(SPHParticle *p);
  virtual void clear();
  virtual void printDiagnostics();

  #if MFluidSolver_USE_OPENVDB
  virtual void exportVDB();
  #endif

  SPHGrid *grid;
};

class UniformGridNeighborSearch :  public GridNeighborSearch {
 public:
  UniformGridNeighborSearch(
    float r,
    const glm::vec3 &gridMin, const glm::vec3 &gridMax,
    float cellSize);
};

class IndexSortedUniformGridNeighborSearch :  public GridNeighborSearch {
 public:
  IndexSortedUniformGridNeighborSearch(
    float r,
    const glm::vec3 &gridMin, const glm::vec3 &gridMax, float cellSize,
    std::vector<SPHParticle> *master,
    bool useZCurve = MFluidSolver_DEFAULT_SPH_NEIGHBORSEARCH_USE_ZCURVE);

  SPHIndexSortedUniformGrid *isuGrid;
};

#endif  // MFLUIDSOLVER_SPHSOLVER_NEIGHBORSEARCH_HPP_
