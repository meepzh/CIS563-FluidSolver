//
//  testNeighborSearch.cpp
//  MFluidSolver

#define BOOST_TEST_MODULE NeighborSearchTests
#include <boost/test/unit_test.hpp>

#include "../src/fluidSolver/sphSolver/neighborSearch.hpp"

BOOST_AUTO_TEST_CASE(NeighborSearch_Naive_BasicSuccess)
{
  NaiveNeighborSearch nSearch(1.f);

  Particle a(glm::vec3(1.f));
  Particle b(glm::vec3(1.f, 1.5f, 1.5f));

  nSearch.addParticle(&a);
  nSearch.addParticle(&b);

  // Check a's neighbors
  std::vector<Particle *> neighbors;
  nSearch.findNeighbors(&a, neighbors);

  BOOST_CHECK_EQUAL(1, neighbors.size());
  BOOST_CHECK_EQUAL(&b, neighbors.at(0));

  // Check b's neighbors
  neighbors.clear();
  nSearch.findNeighbors(&b, neighbors);

  BOOST_CHECK_EQUAL(1, neighbors.size());
  BOOST_CHECK_EQUAL(&a, neighbors.at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_Naive_BasicFail)
{
  NaiveNeighborSearch nSearch(1.f);

  Particle a(glm::vec3(1.f));
  Particle b(glm::vec3(3.f));

  nSearch.addParticle(&a);
  nSearch.addParticle(&b);

  // Check a's neighbors
  std::vector<Particle *> neighbors;
  nSearch.findNeighbors(&a, neighbors);

  BOOST_CHECK_EQUAL(0, neighbors.size());

  // Check b's neighbors
  neighbors.clear();
  nSearch.findNeighbors(&b, neighbors);

  BOOST_CHECK_EQUAL(0, neighbors.size());
}