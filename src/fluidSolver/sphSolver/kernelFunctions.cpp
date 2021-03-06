//  Copyright 2016 Robert Zhou
//
//  kernelFunctions.cpp
//  MFluidSolver

#include "kernelFunctions.hpp"

KernelFunctions::KernelFunctions() {
  setKernelRadius(0.1);
}

void KernelFunctions::setKernelRadius(const double &h) {
  _h = h;
  _h2 = h * h;
  _h3 = _h2 * h;
  _h4 = _h2 * _h2;
  _h5 = _h3 * _h2;
  _h6 = _h3 * _h3;
  _h9 = _h6 * _h3;
}
