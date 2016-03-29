//
//  utils.cpp
//  MFluidSolver

#include "utils.hpp"

namespace MUtils {
  template<typename T> bool fequal(T a, T b, T epsilon) {
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

  void toLowerInplace(std::string &input) {
    for (unsigned int i = 0; i < input.length(); ++i) {
      input[i] = std::tolower(input[i]);
    }
  }

  std::string zeroPad(int number, unsigned int digits) {
    std::ostringstream strstm;

    if (digits <= 1) {
      if (number == 0) return "0";
      strstm << number;
    } else if (digits == 2) {
      if (number == 0) return "00";
      if (number < 10) {
        strstm << "0" << number;
      } else {
        strstm << number;
      }
    } else {
      strstm << number;
      std::string ret = strstm.str();
      int divisor = 10;
      unsigned int count = 1;
      while (count < digits) {
        if (number / divisor != 0) {
          ret = "0" + ret;
        }
        divisor *= 10;
        ++count;
      }
      return ret;
    }

    return strstm.str();
  }

  std::string toHMS(double seconds) {
    std::ostringstream strstm;

    int minutes = seconds / 60;
    int hours = minutes / 60;
    int iseconds = (int)seconds - minutes * 60;
    minutes = minutes % 60;

    double dseconds = seconds - (int)seconds;
    std::string truncateDSeconds = std::to_string(dseconds);
    truncateDSeconds = truncateDSeconds.substr(1);

    strstm << zeroPad(hours, 1) << ":" << zeroPad(minutes, 2) << ":" << zeroPad(iseconds, 2) << truncateDSeconds;

    return strstm.str();
  }
}