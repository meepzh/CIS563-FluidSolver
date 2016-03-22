//
//  testKernelFunctions.cpp
//  MFluidSolver

#define BOOST_TEST_MODULE KernelFunctionsTests
#include <boost/test/unit_test.hpp>

#include "../src/fluidSolver/sphSolver/kernelFunctions.hpp"

BOOST_AUTO_TEST_CASE(KernelFunctionsTest1)
{
  BOOST_CHECK_EQUAL(1, 1);
}

BOOST_AUTO_TEST_CASE(KernelFunctionsTest2)
{
  BOOST_CHECK_EQUAL(0, 0);
}