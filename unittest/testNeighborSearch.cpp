//
//  testNeighborSearch.cpp
//  MFluidSolver

#define BOOST_TEST_MODULE NeighborSearchTests
#include <boost/test/unit_test.hpp>

#include "../src/fluidSolver/sphSolver/neighborSearch.hpp"

BOOST_AUTO_TEST_CASE(FailTest)
{
  BOOST_CHECK_EQUAL(1, 1);
}

BOOST_AUTO_TEST_CASE(PassTest)
{
  BOOST_CHECK_EQUAL(0, 0);
}