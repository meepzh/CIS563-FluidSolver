//
//  testNeighborSearch.cpp
//  MFluidSolver

#define BOOST_TEST_MODULE NeighborSearchTests
#include <boost/test/unit_test.hpp>

#include "../src/fluidSolver/sphSolver/neighborSearch.hpp"

BOOST_AUTO_TEST_CASE(NeighborSearch_Naive_BasicSuccess)
{
  NaiveNeighborSearch nSearch(1.f);

  SPHParticle a(glm::vec3(1.f));
  SPHParticle b(glm::vec3(1.f, 1.5f, 1.5f));

  nSearch.addParticle(&a);
  nSearch.addParticle(&b);

  // Check a's neighbors
  nSearch.findNeighbors(&a);

  BOOST_CHECK_EQUAL(1, a.neighbors().size());
  BOOST_CHECK_EQUAL(&b, a.neighbors().at(0));

  // Check b's neighbors
  nSearch.findNeighbors(&b);

  BOOST_CHECK_EQUAL(1, b.neighbors().size());
  BOOST_CHECK_EQUAL(&a, b.neighbors().at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_Naive_BasicFail)
{
  NaiveNeighborSearch nSearch(1.f);

  SPHParticle a(glm::vec3(1.f));
  SPHParticle b(glm::vec3(3.f));

  nSearch.addParticle(&a);
  nSearch.addParticle(&b);

  // Check a's neighbors
  std::vector<SPHParticle *> neighbors;
  nSearch.findNeighbors(&a);

  BOOST_CHECK_EQUAL(0, a.neighbors().size());

  // Check b's neighbors
  neighbors.clear();
  nSearch.findNeighbors(&b);

  BOOST_CHECK_EQUAL(0, b.neighbors().size());
}