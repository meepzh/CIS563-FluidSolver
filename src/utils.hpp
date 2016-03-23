//
//  utils.hpp
//  MFluidSolver

#ifndef MFLUIDSOLVER_UTILS_HPP_
#define MFLUIDSOLVER_UTILS_HPP_

// Float approximate-equality comparison
template<typename T>
inline bool fequal(T a, T b, T epsilon = 0.0001){
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

#endif /* MFLUIDSOLVER_UTILS_HPP_ */
