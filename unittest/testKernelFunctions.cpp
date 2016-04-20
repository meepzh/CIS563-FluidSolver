//  Copyright 2016 Robert Zhou
//
//  testKernelFunctions.cpp
//  MFluidSolver

#include <boost/test/unit_test.hpp>

#include "fluidSolver/sphSolver/kernelFunctions.hpp"

BOOST_AUTO_TEST_SUITE(KernelFunctionsTests)

BOOST_AUTO_TEST_CASE(KernelFunctions_Poly6_InsideKernelTest) {
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

/*BOOST_AUTO_TEST_CASE(KernelFunctions_Poly6_OutsideKernelTest) {
  KernelFunctions kernelFunctions;
  double out;

  kernelFunctions.setKernelRadius(0.1f);

  out = kernelFunctions.computePoly6(glm::vec3(0.1f, 0.1f, 0.1f));
  BOOST_CHECK_CLOSE(out, 0, 0.001);
}*/

BOOST_AUTO_TEST_CASE(KernelFunctions_Spiky_InsideKernelTest) {
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

/*BOOST_AUTO_TEST_CASE(KernelFunctions_Spiky_OutsideKernelTest) {
  KernelFunctions kernelFunctions;
  double out;

  kernelFunctions.setKernelRadius(0.1f);

  out = kernelFunctions.computeSpiky(glm::vec3(0.1f, 0.1f, 0.1f));
  BOOST_CHECK_CLOSE(out, 0, 0.001);
}*/

BOOST_AUTO_TEST_CASE(KernelFunctions_SpikyGradient_InsideKernelTest) {
  KernelFunctions kernelFunctions;
  glm::vec3 out;

  kernelFunctions.setKernelRadius(0.1f);

  out = kernelFunctions.computeSpikyGradient(glm::vec3(0.05f, 0.05f, 0.05f));
  BOOST_CHECK_CLOSE(out.x, -1484.40, 0.01);
  out = kernelFunctions.computeSpikyGradient(glm::vec3(0.05f, 0.05f, 0.05f));
  BOOST_CHECK_CLOSE(out.y, -1484.40, 0.01);
  out = kernelFunctions.computeSpikyGradient(glm::vec3(0.05f, 0.05f, 0.05f));
  BOOST_CHECK_CLOSE(out.z, -1484.40, 0.01);

  out = kernelFunctions.computeSpikyGradient(glm::vec3(0.03f, 0.04f, 0.05f));
  BOOST_CHECK_CLOSE(out.x, -5213.35, 0.01);
  out = kernelFunctions.computeSpikyGradient(glm::vec3(0.03f, 0.04f, 0.05f));
  BOOST_CHECK_CLOSE(out.y, -6951.13, 0.01);
  out = kernelFunctions.computeSpikyGradient(glm::vec3(0.03f, 0.04f, 0.05f));
  BOOST_CHECK_CLOSE(out.z, -8688.91, 0.01);

  out = kernelFunctions.computeSpikyGradient(glm::vec3(0.003f, 0.002f, 0.001f));
  BOOST_CHECK_CLOSE(out.x, -106413.397, 0.01);
  out = kernelFunctions.computeSpikyGradient(glm::vec3(0.003f, 0.002f, 0.001f));
  BOOST_CHECK_CLOSE(out.y, -70942.265, 0.01);
  out = kernelFunctions.computeSpikyGradient(glm::vec3(0.003f, 0.002f, 0.001f));
  BOOST_CHECK_CLOSE(out.z, -35471.133, 0.01);
}

BOOST_AUTO_TEST_CASE(KernelFunctions_Viscous_InsideKernelTest) {
  KernelFunctions kernelFunctions;
  double out;

  kernelFunctions.setKernelRadius(0.1f);

  out = kernelFunctions.computeViscous(glm::vec3(0.05f, 0.05f, 0.05f));
  BOOST_CHECK_CLOSE(out, 6.18494, 0.01);

  out = kernelFunctions.computeViscous(glm::vec3(0.03f, 0.04f, 0.05f));
  BOOST_CHECK_CLOSE(out, 72.4076, 0.01);

  out = kernelFunctions.computeViscous(glm::vec3(0.003f, 0.002f, 0.001f));
  BOOST_CHECK_CLOSE(out, 29517.8941, 0.01);
}

/*BOOST_AUTO_TEST_CASE(KernelFunctions_Viscous_OutsideKernelTest) {
  KernelFunctions kernelFunctions;
  double out;

  kernelFunctions.setKernelRadius(0.1f);

  out = kernelFunctions.computeViscous(glm::vec3(0.1f, 0.1f, 0.1f));
  BOOST_CHECK_CLOSE(out, 0, 0.001);
}*/

BOOST_AUTO_TEST_CASE(KernelFunctions_ViscousLaplacian_Test) {
  KernelFunctions kernelFunctions;
  double out;

  kernelFunctions.setKernelRadius(0.1f);

  out = kernelFunctions.computeViscousLaplacian(glm::vec3(0.05f, 0.05f, 0.05f));
  BOOST_CHECK_CLOSE(out, 191905.05, 1);

  out = kernelFunctions.computeViscousLaplacian(glm::vec3(0.03f, 0.04f, 0.05f));
  BOOST_CHECK_CLOSE(out, 419538.32, 1);

  out =
    kernelFunctions.computeViscousLaplacian(glm::vec3(0.003f, 0.002f, 0.001f));
  BOOST_CHECK_CLOSE(out, 1378799, 1);
}

BOOST_AUTO_TEST_SUITE_END()
