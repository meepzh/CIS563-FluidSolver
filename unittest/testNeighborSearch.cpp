//
//  testNeighborSearch.cpp
//  MFluidSolver

#include <boost/test/unit_test.hpp>
#include <ctime>

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
  UniformGridNeighborSearch nSearch(0.5f, glm::vec3(0), glm::vec3(1), 0.5f);

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
  UniformGridNeighborSearch nSearch(0.5f, glm::vec3(0), glm::vec3(1), 0.5f);

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
  UniformGridNeighborSearch nSearch(0.5f, glm::vec3(0), glm::vec3(1), 0.5f);

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
  UniformGridNeighborSearch nSearch(0.5f, glm::vec3(0), glm::vec3(1), 0.5f);

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
  UniformGridNeighborSearch nSearch(0.2f, glm::vec3(0), glm::vec3(1), 0.2f);

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
  UniformGridNeighborSearch nSearch(0.2f, glm::vec3(0), glm::vec3(1), 0.2f);

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
  UniformGridNeighborSearch nSearch(0.2f, glm::vec3(0), glm::vec3(1), 0.2f);

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
  UniformGridNeighborSearch nSearch(0.5f, glm::vec3(2), glm::vec3(3), 0.5f);

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
  UniformGridNeighborSearch nSearch(0.5f, glm::vec3(-3), glm::vec3(-2), 0.5f);

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

BOOST_AUTO_TEST_CASE(NeighborSearch_Naive_StressTest)
{
  NaiveNeighborSearch nSearch(0.2f);
  std::vector<SPHParticle *> particles;

  // Init particles
  glm::vec3 minBound(0);
  glm::vec3 maxBound(2.5);
  float particleSeparation = 0.1f;
  minBound += particleSeparation / 2.f;
  maxBound -= particleSeparation / 2.f;
  for (float i = minBound.x; i <= maxBound.x; i += particleSeparation) {
    for (float j = minBound.y; j <= maxBound.y; j += particleSeparation) {
      for (float k = minBound.z; k <= maxBound.z; k += particleSeparation) {
        SPHParticle *p = new SPHParticle(glm::vec3(i, j, k));
        particles.push_back(p);
        nSearch.addParticle(p);
      }
    }
  }

  std::clock_t startTime = std::clock();
  for (SPHParticle *p : particles) {
    nSearch.findNeighbors(p);
  }
  clock_t endTime = clock();

  double timeInSeconds = (endTime - startTime) / (double) CLOCKS_PER_SEC;
  printf("INFO: Naive neighbor search took %.6f seconds for %d particles\n", timeInSeconds, particles.size());
}

BOOST_AUTO_TEST_CASE(NeighborSearch_StandardGrid_StressTest)
{
  glm::vec3 minBound(0);
  glm::vec3 maxBound(2.5);

  UniformGridNeighborSearch nSearch(0.2f, minBound, maxBound, 0.2f);
  std::vector<SPHParticle *> particles;

  // Init particles
  float particleSeparation = 0.1f;
  minBound += particleSeparation / 2.f;
  maxBound -= particleSeparation / 2.f;
  for (float i = minBound.x; i <= maxBound.x; i += particleSeparation) {
    for (float j = minBound.y; j <= maxBound.y; j += particleSeparation) {
      for (float k = minBound.z; k <= maxBound.z; k += particleSeparation) {
        SPHParticle *p = new SPHParticle(glm::vec3(i, j, k));
        particles.push_back(p);
        nSearch.addParticle(p);
      }
    }
  }

  std::clock_t startTime = std::clock();
  for (SPHParticle *p : particles) {
    nSearch.findNeighbors(p);
  }
  clock_t endTime = clock();

  double timeInSeconds = (endTime - startTime) / (double) CLOCKS_PER_SEC;
  printf("INFO: Uniform grid neighbor search took %.6f seconds for %d particles\n", timeInSeconds, particles.size());
}

BOOST_AUTO_TEST_SUITE_END()