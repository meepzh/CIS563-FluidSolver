//
//  testNeighborSearch.cpp
//  MFluidSolver

#include <boost/test/unit_test.hpp>

#include "../src/fluidSolver/sphSolver/neighborSearch.hpp"

BOOST_AUTO_TEST_SUITE(NeighborSearchTests)

BOOST_AUTO_TEST_CASE(NeighborSearch_Naive_BasicSuccess)
{
  NaiveNeighborSearch nSearch(1.f);

  SPHParticle a(glm::vec3(1.f));
  SPHParticle b(glm::vec3(1.f, 1.5f, 1.5f));

  nSearch.addParticle(&a);
  nSearch.addParticle(&b);

  // Check a's neighbors
  nSearch.findNeighbors(&a);

  BOOST_CHECK_EQUAL(1, a.neighbors()->size());
  if (a.neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(&b, a.neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(&b);

  BOOST_CHECK_EQUAL(1, b.neighbors()->size());
  if (b.neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(&a, b.neighbors()->at(0));
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

  BOOST_CHECK_EQUAL(0, a.neighbors()->size());

  // Check b's neighbors
  neighbors.clear();
  nSearch.findNeighbors(&b);

  BOOST_CHECK_EQUAL(0, b.neighbors()->size());
}

BOOST_AUTO_TEST_CASE(NeighborSearch_StandardGrid_SameCell)
{
  StandardGridNeighborSearch nSearch(0.5f, glm::vec3(0), glm::vec3(1), 0.5f);

  SPHParticle a(glm::vec3(0.25f));
  SPHParticle b(glm::vec3(0.2f));

  nSearch.addParticle(&a);
  nSearch.addParticle(&b);

  // Check a's neighbors
  nSearch.findNeighbors(&a);

  BOOST_CHECK_EQUAL(1, a.neighbors()->size());
  if (a.neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(&b, a.neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(&b);

  BOOST_CHECK_EQUAL(1, b.neighbors()->size());
  if (b.neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(&a, b.neighbors()->at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_StandardGrid_DifferentCellStillNeighborsX)
{
  StandardGridNeighborSearch nSearch(0.5f, glm::vec3(0), glm::vec3(1), 0.5f);

  SPHParticle a(glm::vec3(0.25f));
  SPHParticle b(glm::vec3(0.6f, 0.25f, 0.25f));

  nSearch.addParticle(&a);
  nSearch.addParticle(&b);

  // Check a's neighbors
  nSearch.findNeighbors(&a);

  BOOST_CHECK_EQUAL(1, a.neighbors()->size());
  if (a.neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(&b, a.neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(&b);

  BOOST_CHECK_EQUAL(1, b.neighbors()->size());
  if (b.neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(&a, b.neighbors()->at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_StandardGrid_DifferentCellStillNeighborsY)
{
  StandardGridNeighborSearch nSearch(0.5f, glm::vec3(0), glm::vec3(1), 0.5f);

  SPHParticle a(glm::vec3(0.25f));
  SPHParticle b(glm::vec3(0.25f, 0.6f, 0.25f));

  nSearch.addParticle(&a);
  nSearch.addParticle(&b);

  // Check a's neighbors
  nSearch.findNeighbors(&a);

  BOOST_CHECK_EQUAL(1, a.neighbors()->size());
  if (a.neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(&b, a.neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(&b);

  BOOST_CHECK_EQUAL(1, b.neighbors()->size());
  if (b.neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(&a, b.neighbors()->at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_StandardGrid_DifferentCellStillNeighborsZ)
{
  StandardGridNeighborSearch nSearch(0.5f, glm::vec3(0), glm::vec3(1), 0.5f);

  SPHParticle a(glm::vec3(0.25f));
  SPHParticle b(glm::vec3(0.25f, 0.25f, 0.6f));

  nSearch.addParticle(&a);
  nSearch.addParticle(&b);

  // Check a's neighbors
  nSearch.findNeighbors(&a);

  BOOST_CHECK_EQUAL(1, a.neighbors()->size());
  if (a.neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(&b, a.neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(&b);

  BOOST_CHECK_EQUAL(1, b.neighbors()->size());
  if (b.neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(&a, b.neighbors()->at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_StandardGrid_DifferentCellNotNeighborsX)
{
  StandardGridNeighborSearch nSearch(0.2f, glm::vec3(0), glm::vec3(1), 0.2f);

  SPHParticle a(glm::vec3(0.1f));
  SPHParticle b(glm::vec3(0.9f, 0.1f, 0.1f));

  nSearch.addParticle(&a);
  nSearch.addParticle(&b);

  // Check a's neighbors
  nSearch.findNeighbors(&a);

  BOOST_CHECK_EQUAL(0, a.neighbors()->size());

  // Check b's neighbors
  nSearch.findNeighbors(&b);

  BOOST_CHECK_EQUAL(0, b.neighbors()->size());
}

BOOST_AUTO_TEST_CASE(NeighborSearch_StandardGrid_DifferentCellNotNeighborsY)
{
  StandardGridNeighborSearch nSearch(0.2f, glm::vec3(0), glm::vec3(1), 0.2f);

  SPHParticle a(glm::vec3(0.1f));
  SPHParticle b(glm::vec3(0.1f, 0.9f, 0.1f));

  nSearch.addParticle(&a);
  nSearch.addParticle(&b);

  // Check a's neighbors
  nSearch.findNeighbors(&a);

  BOOST_CHECK_EQUAL(0, a.neighbors()->size());

  // Check b's neighbors
  nSearch.findNeighbors(&b);

  BOOST_CHECK_EQUAL(0, b.neighbors()->size());
}

BOOST_AUTO_TEST_CASE(NeighborSearch_StandardGrid_DifferentCellNotNeighborsZ)
{
  StandardGridNeighborSearch nSearch(0.2f, glm::vec3(0), glm::vec3(1), 0.2f);

  SPHParticle a(glm::vec3(0.1f));
  SPHParticle b(glm::vec3(0.1f, 0.1f, 0.9f));

  nSearch.addParticle(&a);
  nSearch.addParticle(&b);

  // Check a's neighbors
  nSearch.findNeighbors(&a);

  BOOST_CHECK_EQUAL(0, a.neighbors()->size());

  // Check b's neighbors
  nSearch.findNeighbors(&b);

  BOOST_CHECK_EQUAL(0, b.neighbors()->size());
}

BOOST_AUTO_TEST_CASE(NeighborSearch_StandardGrid_DifferentCellStillNeighborsPositiveOffsetX)
{
  StandardGridNeighborSearch nSearch(0.5f, glm::vec3(2), glm::vec3(3), 0.5f);

  SPHParticle a(glm::vec3(2.25f));
  SPHParticle b(glm::vec3(2.6f, 2.25f, 2.25f));

  nSearch.addParticle(&a);
  nSearch.addParticle(&b);

  // Check a's neighbors
  nSearch.findNeighbors(&a);

  BOOST_CHECK_EQUAL(1, a.neighbors()->size());
  if (a.neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(&b, a.neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(&b);

  BOOST_CHECK_EQUAL(1, b.neighbors()->size());
  if (b.neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(&a, b.neighbors()->at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_StandardGrid_DifferentCellStillNeighborsNegativeOffsetX)
{
  StandardGridNeighborSearch nSearch(0.5f, glm::vec3(-3), glm::vec3(-2), 0.5f);

  SPHParticle a(glm::vec3(-2.75f));
  SPHParticle b(glm::vec3(-2.4f, -2.75f, -2.75f));

  nSearch.addParticle(&a);
  nSearch.addParticle(&b);

  // Check a's neighbors
  nSearch.findNeighbors(&a);

  BOOST_CHECK_EQUAL(1, a.neighbors()->size());
  if (a.neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(&b, a.neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(&b);

  BOOST_CHECK_EQUAL(1, b.neighbors()->size());
  if (b.neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(&a, b.neighbors()->at(0));
}

BOOST_AUTO_TEST_SUITE_END()