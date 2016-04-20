//  Copyright 2016 Robert Zhou
//
//  testNeighborSearch.cpp
//  MFluidSolver

#include <ctime>
#include <iostream>

#include <boost/test/unit_test.hpp>

#include "fluidSolver/sphSolver/neighborSearch.hpp"

BOOST_AUTO_TEST_SUITE(NeighborSearchTests)

/*******************************************/
/****************** Naive ******************/
/*******************************************/

BOOST_AUTO_TEST_CASE(NeighborSearch_Naive_BasicSuccess) {
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

BOOST_AUTO_TEST_CASE(NeighborSearch_Naive_BasicFail) {
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

BOOST_AUTO_TEST_CASE(NeighborSearch_Naive_StressTest) {
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

  double timeInSeconds =
    (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC);
  std::cout << "INFO: Naive neighbor search took " <<
    timeInSeconds << " seconds for " <<
    particles.size() << " particles\n" << std::endl;
}

/*************************************************/
/****************** UniformGrid ******************/
/*************************************************/

BOOST_AUTO_TEST_CASE(NeighborSearch_UniformGrid_SameCell) {
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

BOOST_AUTO_TEST_CASE(NeighborSearch_UniformGrid_DifferentCellStillNeighborsX) {
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

BOOST_AUTO_TEST_CASE(NeighborSearch_UniformGrid_DifferentCellStillNeighborsY) {
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

BOOST_AUTO_TEST_CASE(NeighborSearch_UniformGrid_DifferentCellStillNeighborsZ) {
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

BOOST_AUTO_TEST_CASE(NeighborSearch_UniformGrid_DifferentCellNotNeighborsX) {
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

BOOST_AUTO_TEST_CASE(NeighborSearch_UniformGrid_DifferentCellNotNeighborsY) {
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

BOOST_AUTO_TEST_CASE(NeighborSearch_UniformGrid_DifferentCellNotNeighborsZ) {
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

BOOST_AUTO_TEST_CASE(NeighborSearch_UniformGrid_DifferentCellStillNeighborsPositiveOffsetX) {
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

BOOST_AUTO_TEST_CASE(NeighborSearch_UniformGrid_DifferentCellStillNeighborsNegativeOffsetX) {
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

BOOST_AUTO_TEST_CASE(NeighborSearch_UniformGrid_StressTest) {
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

  double timeInSeconds =
    (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC);
  std:: cout << "INFO: Uniform grid neighbor search took" <<
    timeInSeconds << " seconds for " <<
    particles.size() << " particles\n" << std::endl;
}

/************************************************************/
/****************** IndexSortedUniformGrid ******************/
/************************************************************/

BOOST_AUTO_TEST_CASE(NeighborSearch_IndexSortedUniformGrid_SameCell) {
  std::vector<SPHParticle> pList;
  pList.push_back(SPHParticle(glm::vec3(0.25f)));
  pList.push_back(SPHParticle(glm::vec3(0.2f)));

  SPHParticle *a = &(pList.at(0));
  SPHParticle *b = &(pList.at(1));

  IndexSortedUniformGridNeighborSearch nSearch(
    0.5f, glm::vec3(0), glm::vec3(1), 0.5f, &pList, false);
  nSearch.isuGrid->resetAndFillCells();

  // Check a's neighbors
  nSearch.findNeighbors(a);

  BOOST_CHECK_EQUAL(1, a->neighbors()->size());
  if (a->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(b, a->neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(b);

  BOOST_CHECK_EQUAL(1, b->neighbors()->size());
  if (b->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(a, b->neighbors()->at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_IndexSortedUniformGrid_DifferentCellStillNeighborsX) {
  std::vector<SPHParticle> pList;
  pList.push_back(SPHParticle(glm::vec3(0.25f)));
  pList.push_back(SPHParticle(glm::vec3(0.6f, 0.25f, 0.25f)));

  SPHParticle *a = &(pList.at(0));
  SPHParticle *b = &(pList.at(1));

  IndexSortedUniformGridNeighborSearch nSearch(
    0.5f, glm::vec3(0), glm::vec3(1), 0.5f, &pList, false);
  nSearch.isuGrid->resetAndFillCells();

  // Check a's neighbors
  nSearch.findNeighbors(a);

  BOOST_CHECK_EQUAL(1, a->neighbors()->size());
  if (a->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(b, a->neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(b);

  BOOST_CHECK_EQUAL(1, b->neighbors()->size());
  if (b->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(a, b->neighbors()->at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_IndexSortedUniformGrid_DifferentCellStillNeighborsY) {
  std::vector<SPHParticle> pList;
  pList.push_back(SPHParticle(glm::vec3(0.25f)));
  pList.push_back(SPHParticle(glm::vec3(0.26f, 0.6f, 0.25f)));

  SPHParticle *a = &(pList.at(0));
  SPHParticle *b = &(pList.at(1));

  IndexSortedUniformGridNeighborSearch nSearch(
    0.5f, glm::vec3(0), glm::vec3(1), 0.5f, &pList, false);
  nSearch.isuGrid->resetAndFillCells();

  // Check a's neighbors
  nSearch.findNeighbors(a);

  BOOST_CHECK_EQUAL(1, a->neighbors()->size());
  if (a->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(b, a->neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(b);

  BOOST_CHECK_EQUAL(1, b->neighbors()->size());
  if (b->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(a, b->neighbors()->at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_IndexSortedUniformGrid_DifferentCellStillNeighborsZ) {
  std::vector<SPHParticle> pList;
  pList.push_back(SPHParticle(glm::vec3(0.25f)));
  pList.push_back(SPHParticle(glm::vec3(0.26f, 0.25f, 0.6f)));

  SPHParticle *a = &(pList.at(0));
  SPHParticle *b = &(pList.at(1));

  IndexSortedUniformGridNeighborSearch nSearch(
    0.5f, glm::vec3(0), glm::vec3(1), 0.5f, &pList, false);
  nSearch.isuGrid->resetAndFillCells();

  // Check a's neighbors
  nSearch.findNeighbors(a);

  BOOST_CHECK_EQUAL(1, a->neighbors()->size());
  if (a->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(b, a->neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(b);

  BOOST_CHECK_EQUAL(1, b->neighbors()->size());
  if (b->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(a, b->neighbors()->at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_IndexSortedUniformGrid_DifferentCellNotNeighborsX) {
  std::vector<SPHParticle> pList;
  pList.push_back(SPHParticle(glm::vec3(0.1f)));
  pList.push_back(SPHParticle(glm::vec3(0.9f, 0.1f, 0.1f)));

  SPHParticle *a = &(pList.at(0));
  SPHParticle *b = &(pList.at(1));

  IndexSortedUniformGridNeighborSearch nSearch(
    0.2f, glm::vec3(0), glm::vec3(1), 0.2f, &pList, false);
  nSearch.isuGrid->resetAndFillCells();

  // Check a's neighbors
  nSearch.findNeighbors(a);

  BOOST_CHECK_EQUAL(0, a->neighbors()->size());

  // Check b's neighbors
  nSearch.findNeighbors(b);

  BOOST_CHECK_EQUAL(0, b->neighbors()->size());
}

BOOST_AUTO_TEST_CASE(NeighborSearch_IndexSortedUniformGrid_DifferentCellNotNeighborsY) {
  std::vector<SPHParticle> pList;
  pList.push_back(SPHParticle(glm::vec3(0.1f)));
  pList.push_back(SPHParticle(glm::vec3(0.1f, 0.9f, 0.1f)));

  SPHParticle *a = &(pList.at(0));
  SPHParticle *b = &(pList.at(1));

  IndexSortedUniformGridNeighborSearch nSearch(
    0.2f, glm::vec3(0), glm::vec3(1), 0.2f, &pList, false);
  nSearch.isuGrid->resetAndFillCells();

  // Check a's neighbors
  nSearch.findNeighbors(a);

  BOOST_CHECK_EQUAL(0, a->neighbors()->size());

  // Check b's neighbors
  nSearch.findNeighbors(b);

  BOOST_CHECK_EQUAL(0, b->neighbors()->size());
}

BOOST_AUTO_TEST_CASE(NeighborSearch_IndexSortedUniformGrid_DifferentCellNotNeighborsZ) {
  std::vector<SPHParticle> pList;
  pList.push_back(SPHParticle(glm::vec3(0.1f)));
  pList.push_back(SPHParticle(glm::vec3(0.1f, 0.1f, 0.9f)));

  SPHParticle *a = &(pList.at(0));
  SPHParticle *b = &(pList.at(1));

  IndexSortedUniformGridNeighborSearch nSearch(
    0.2f, glm::vec3(0), glm::vec3(1), 0.2f, &pList, false);
  nSearch.isuGrid->resetAndFillCells();

  // Check a's neighbors
  nSearch.findNeighbors(a);

  BOOST_CHECK_EQUAL(0, a->neighbors()->size());

  // Check b's neighbors
  nSearch.findNeighbors(b);

  BOOST_CHECK_EQUAL(0, b->neighbors()->size());
}

/*************************************************************/
/****************** ZIndexSortedUniformGrid ******************/
/*************************************************************/

BOOST_AUTO_TEST_CASE(NeighborSearch_ZIndexSortedUniformGrid_SameCell) {
  std::vector<SPHParticle> pList;
  pList.push_back(SPHParticle(glm::vec3(0.25f)));
  pList.push_back(SPHParticle(glm::vec3(0.2f)));

  SPHParticle *a = &(pList.at(0));
  SPHParticle *b = &(pList.at(1));

  IndexSortedUniformGridNeighborSearch nSearch(
    0.5f, glm::vec3(0), glm::vec3(1), 0.5f, &pList, true);
  nSearch.isuGrid->resetAndFillCells();

  // Check a's neighbors
  nSearch.findNeighbors(a);

  BOOST_CHECK_EQUAL(1, a->neighbors()->size());
  if (a->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(b, a->neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(b);

  BOOST_CHECK_EQUAL(1, b->neighbors()->size());
  if (b->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(a, b->neighbors()->at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_ZIndexSortedUniformGrid_DifferentCellStillNeighborsX) {
  std::vector<SPHParticle> pList;
  pList.push_back(SPHParticle(glm::vec3(0.25f)));
  pList.push_back(SPHParticle(glm::vec3(0.6f, 0.25f, 0.25f)));

  SPHParticle *a = &(pList.at(0));
  SPHParticle *b = &(pList.at(1));

  IndexSortedUniformGridNeighborSearch nSearch(
    0.5f, glm::vec3(0), glm::vec3(1), 0.5f, &pList, true);
  nSearch.isuGrid->resetAndFillCells();

  // Check a's neighbors
  nSearch.findNeighbors(a);

  BOOST_CHECK_EQUAL(1, a->neighbors()->size());
  if (a->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(b, a->neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(b);

  BOOST_CHECK_EQUAL(1, b->neighbors()->size());
  if (b->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(a, b->neighbors()->at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_ZIndexSortedUniformGrid_DifferentCellStillNeighborsY) {
  std::vector<SPHParticle> pList;
  pList.push_back(SPHParticle(glm::vec3(0.25f)));
  pList.push_back(SPHParticle(glm::vec3(0.26f, 0.6f, 0.25f)));

  SPHParticle *a = &(pList.at(0));
  SPHParticle *b = &(pList.at(1));

  IndexSortedUniformGridNeighborSearch nSearch(
    0.5f, glm::vec3(0), glm::vec3(1), 0.5f, &pList, true);
  nSearch.isuGrid->resetAndFillCells();

  // Check a's neighbors
  nSearch.findNeighbors(a);

  BOOST_CHECK_EQUAL(1, a->neighbors()->size());
  if (a->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(b, a->neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(b);

  BOOST_CHECK_EQUAL(1, b->neighbors()->size());
  if (b->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(a, b->neighbors()->at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_ZIndexSortedUniformGrid_DifferentCellStillNeighborsZ) {
  std::vector<SPHParticle> pList;
  pList.push_back(SPHParticle(glm::vec3(0.25f)));
  pList.push_back(SPHParticle(glm::vec3(0.26f, 0.25f, 0.6f)));

  SPHParticle *a = &(pList.at(0));
  SPHParticle *b = &(pList.at(1));

  IndexSortedUniformGridNeighborSearch nSearch(
    0.5f, glm::vec3(0), glm::vec3(1), 0.5f, &pList, true);
  nSearch.isuGrid->resetAndFillCells();

  // Check a's neighbors
  nSearch.findNeighbors(a);

  BOOST_CHECK_EQUAL(1, a->neighbors()->size());
  if (a->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(b, a->neighbors()->at(0));

  // Check b's neighbors
  nSearch.findNeighbors(b);

  BOOST_CHECK_EQUAL(1, b->neighbors()->size());
  if (b->neighbors()->size() > 0)
    BOOST_CHECK_EQUAL(a, b->neighbors()->at(0));
}

BOOST_AUTO_TEST_CASE(NeighborSearch_ZIndexSortedUniformGrid_DifferentCellNotNeighborsX) {
  std::vector<SPHParticle> pList;
  pList.push_back(SPHParticle(glm::vec3(0.1f)));
  pList.push_back(SPHParticle(glm::vec3(0.9f, 0.1f, 0.1f)));

  SPHParticle *a = &(pList.at(0));
  SPHParticle *b = &(pList.at(1));

  IndexSortedUniformGridNeighborSearch nSearch(
    0.2f, glm::vec3(0), glm::vec3(1), 0.2f, &pList, true);
  nSearch.isuGrid->resetAndFillCells();

  // Check a's neighbors
  nSearch.findNeighbors(a);

  BOOST_CHECK_EQUAL(0, a->neighbors()->size());

  // Check b's neighbors
  nSearch.findNeighbors(b);

  BOOST_CHECK_EQUAL(0, b->neighbors()->size());
}

BOOST_AUTO_TEST_CASE(NeighborSearch_ZIndexSortedUniformGrid_DifferentCellNotNeighborsY) {
  std::vector<SPHParticle> pList;
  pList.push_back(SPHParticle(glm::vec3(0.1f)));
  pList.push_back(SPHParticle(glm::vec3(0.1f, 0.9f, 0.1f)));

  SPHParticle *a = &(pList.at(0));
  SPHParticle *b = &(pList.at(1));

  IndexSortedUniformGridNeighborSearch nSearch(
    0.2f, glm::vec3(0), glm::vec3(1), 0.2f, &pList, true);
  nSearch.isuGrid->resetAndFillCells();

  // Check a's neighbors
  nSearch.findNeighbors(a);

  BOOST_CHECK_EQUAL(0, a->neighbors()->size());

  // Check b's neighbors
  nSearch.findNeighbors(b);

  BOOST_CHECK_EQUAL(0, b->neighbors()->size());
}

BOOST_AUTO_TEST_CASE(NeighborSearch_ZIndexSortedUniformGrid_DifferentCellNotNeighborsZ) {
  std::vector<SPHParticle> pList;
  pList.push_back(SPHParticle(glm::vec3(0.1f)));
  pList.push_back(SPHParticle(glm::vec3(0.1f, 0.1f, 0.9f)));

  SPHParticle *a = &(pList.at(0));
  SPHParticle *b = &(pList.at(1));

  IndexSortedUniformGridNeighborSearch nSearch(
    0.2f, glm::vec3(0), glm::vec3(1), 0.2f, &pList, true);
  nSearch.isuGrid->resetAndFillCells();

  // Check a's neighbors
  nSearch.findNeighbors(a);

  BOOST_CHECK_EQUAL(0, a->neighbors()->size());

  // Check b's neighbors
  nSearch.findNeighbors(b);

  BOOST_CHECK_EQUAL(0, b->neighbors()->size());
}

BOOST_AUTO_TEST_SUITE_END()
