//
//  utils.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_UTILS_HPP_
#define MFLUIDSOLVER_UTILS_HPP_

#include <algorithm>
#include <locale>
#include <string>
#include <sstream>

namespace MUtils {
  // Float approximate-equality comparison
  template<typename T> bool fequal(T a, T b, T epsilon = 0.0001) {
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

  // Based on pseudocode from https://en.wikipedia.org/wiki/Insertion_sort
  template<class RandomIt, class Compare> void insertionSort(RandomIt first, RandomIt last, Compare comp) {
    for (RandomIt i = first + 1; i < last; ++i) {
      for (RandomIt j = i; j > first && comp(*j, *(j - 1)); --j) {
        std::iter_swap(j, j - 1);
      }
    }
  }

  // Non-template functions
  std::string toHMS(double seconds);
  void toLowerInplace(std::string &input);
  std::string zeroPad(int number, unsigned int digits);
}

#endif /* MFLUIDSOLVER_UTILS_HPP_ */
