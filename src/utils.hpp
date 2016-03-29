//
//  utils.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_UTILS_HPP_
#define MFLUIDSOLVER_UTILS_HPP_

#include <locale>
#include <string>

namespace MUtils {
  // Float approximate-equality comparison
  template<typename T> inline bool fequal(T a, T b, T epsilon = 0.0001){
    if (a == b) {
      // Shortcut
      return true;
    }

    const T diff = std::abs(a - b);
    if (a * b == 0) {
      // a or b or both are zero; relative error is not meaningful here
      return diff < (epsilon * epsilon);
    }

    return diff / (std::abs(a) + std::abs(b)) < epsilon;
  }

  void inline toLowerInplace(std::string &input) {
    for (unsigned int i = 0; i < input.length(); ++i) {
      input[i] = std::tolower(input[i]);
    }
  }
}

#endif /* MFLUIDSOLVER_UTILS_HPP_ */
