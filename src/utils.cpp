//  Copyright 2016 Robert Zhou
//
//  utils.cpp
//  MFluidSolver

#include "utils.hpp"

#include <iomanip>
#include <locale>
#include <sstream>
#include <string>

namespace MUtils {
  std::string toHMS(double seconds) {
    std::ostringstream strstm;

    int minutes = seconds / 60;
    int hours = minutes / 60;
    int iseconds = static_cast<int>(seconds - minutes * 60);
    minutes = minutes % 60;

    double dseconds = seconds - static_cast<int>(seconds);
    std::string truncateDSeconds = std::to_string(dseconds);
    truncateDSeconds = truncateDSeconds.substr(1);

    strstm << zeroPad(hours, 1) << ":" <<
              zeroPad(minutes, 2) << ":" <<
              zeroPad(iseconds, 2) <<
              truncateDSeconds;

    return strstm.str();
  }

  void toLowerInplace(std::string *input) {
    for (unsigned int i = 0; i < input->length(); ++i) {
      (*input)[i] = std::tolower((*input)[i]);
    }
  }

  std::string zeroPad(int number, unsigned int digits) {
    std::ostringstream ss;
    ss << std::setw(digits) << std::setfill('0') << number;
    return ss.str();
  }
}  // namespace MUtils
