//
//  testKernelFunctions.cpp
//  MFluidSolver

#include <boost/test/unit_test.hpp>

#include "../src/fluidSolver/sphSolver/kernelFunctions.hpp"

BOOST_AUTO_TEST_SUITE(KernelFunctionsTests)

BOOST_AUTO_TEST_CASE(KernelFunctions_Poly6_InsideKernelTest)
{
  KernelFunctions kernelFunctions;
  double out;

  kernelFunctions.setKernelRadius(0.1f);

  out = kernelFunctions.computePoly6(glm::vec3(0.05f, 0.05f, 0.05f));
  BOOST_CHECK_CLOSE(out, 24.4794, 0.01);

  out = kernelFunctions.computePoly6(glm::vec3(0.03f, 0.04f, 0.05f));
  BOOST_CHECK_CLOSE(out, 195.835, 0.01);

  out = kernelFunctions.computePoly6(glm::vec3(0.003f, 0.002f, 0.001f));
  BOOST_CHECK_CLOSE(out, 1560.11, 0.01);
}

BOOST_AUTO_TEST_CASE(KernelFunctions_Poly6_OutsideKernelTest)
{
  KernelFunctions kernelFunctions;
  double out;

  kernelFunctions.setKernelRadius(0.1f);

  out = kernelFunctions.computePoly6(glm::vec3(0.1f, 0.1f, 0.1f));
  BOOST_CHECK_CLOSE(out, 0, 0.001);
}

BOOST_AUTO_TEST_CASE(KernelFunctions_Spiky_InsideKernelTest)
{
  KernelFunctions kernelFunctions;
  double out;

  kernelFunctions.setKernelRadius(0.1f);

  out = kernelFunctions.computeSpiky(glm::vec3(0.05f, 0.05f, 0.05f));
  BOOST_CHECK_CLOSE(out, 11.4819, 0.01);

  out = kernelFunctions.computeSpiky(glm::vec3(0.03f, 0.04f, 0.05f));
  BOOST_CHECK_CLOSE(out, 119.969, 0.01);

  out = kernelFunctions.computeSpiky(glm::vec3(0.003f, 0.002f, 0.001f));
  BOOST_CHECK_CLOSE(out, 4258.50, 0.01);
}

BOOST_AUTO_TEST_CASE(KernelFunctions_Spiky_OutsideKernelTest)
{
  KernelFunctions kernelFunctions;
  double out;

  kernelFunctions.setKernelRadius(0.1f);

  out = kernelFunctions.computeSpiky(glm::vec3(0.1f, 0.1f, 0.1f));
  BOOST_CHECK_CLOSE(out, 0, 0.001);
}

BOOST_AUTO_TEST_SUITE_END()