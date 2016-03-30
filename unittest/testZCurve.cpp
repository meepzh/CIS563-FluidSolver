//
//  testZCurve.cpp
//  MFluidSolver

#include <boost/test/unit_test.hpp>

#include "../src/fluidSolver/zCurve.hpp"

BOOST_AUTO_TEST_SUITE(ZCurveTests)

BOOST_AUTO_TEST_CASE(ZCurve_NegativeXFail)
{
  ZCurve z;
  BOOST_TEST_MESSAGE("Expecting negative bounds exception");
  BOOST_CHECK_THROW(z.initWithMax(glm::ivec3(-1, 10, 10)), ZCurveNegativeBoundsException);
}

BOOST_AUTO_TEST_CASE(ZCurve_NegativeYFail)
{
  ZCurve z;
  BOOST_TEST_MESSAGE("Expecting negative bounds exception");
  BOOST_CHECK_THROW(z.initWithMax(glm::ivec3(10, -1, 10)), ZCurveNegativeBoundsException);
}

BOOST_AUTO_TEST_CASE(ZCurve_NegativeZFail)
{
  ZCurve z;
  BOOST_TEST_MESSAGE("Expecting negative bounds exception");
  BOOST_CHECK_THROW(z.initWithMax(glm::ivec3(10, 10, -1)), ZCurveNegativeBoundsException);
}

BOOST_AUTO_TEST_CASE(ZCurve_LargeXFail)
{
  ZCurve z;
  BOOST_TEST_MESSAGE("Expecting exceeded bounds exception");
  BOOST_CHECK_THROW(z.initWithMax(glm::ivec3(2050, 10, 10)), ZCurveTooLargeException);
}

BOOST_AUTO_TEST_CASE(ZCurve_LargeYFail)
{
  ZCurve z;
  BOOST_TEST_MESSAGE("Expecting exceeded bounds exception");
  BOOST_CHECK_THROW(z.initWithMax(glm::ivec3(10, 2050, 10)), ZCurveTooLargeException);
}

BOOST_AUTO_TEST_CASE(ZCurve_LargeZFail)
{
  ZCurve z;
  BOOST_TEST_MESSAGE("Expecting exceeded bounds exception");
  BOOST_CHECK_THROW(z.initWithMax(glm::ivec3(10, 10, 2050)), ZCurveTooLargeException);
}

BOOST_AUTO_TEST_CASE(ZCurve_Small1)
{
  ZCurve z;
  BOOST_CHECK_EQUAL(z.initWithMax(glm::ivec3(1, 2, 3)), 53);
}

BOOST_AUTO_TEST_SUITE_END()