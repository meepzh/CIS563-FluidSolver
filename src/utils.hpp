//
//  utils.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_UTILS_HPP_
#define MFLUIDSOLVER_UTILS_HPP_

#include <locale>
#include <string>
#include <sstream>

namespace MUtils {
  // Float approximate-equality comparison
  template<typename T> bool fequal(T a, T b, T epsilon = 0.0001);

  void toLowerInplace(std::string &input);

  std::string zeroPad(int number, unsigned int digits);

  std::string toHMS(double seconds);
}

#endif /* MFLUIDSOLVER_UTILS_HPP_ */
