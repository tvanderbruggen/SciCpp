#ifndef SCICPP_MATHS_MATHS
#define SCICPP_MATHS_MATHS

#include <array>
#include <limits>
#include <vector>

namespace scicpp {
namespace maths {

//---------------------------------------------------------------------------------
// fabs
//---------------------------------------------------------------------------------

// https://codereview.stackexchange.com/questions/60140/generic-absolute-value-function

template <typename T>
constexpr T fabs(const T &x) {
    static_assert(std::is_floating_point<T>::value);

// Handles negative zero
// Could use std::fpclassify(x) == FP_ZERO
// to quiet warning, but this is not constexpr.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    if (x == 0.) {
        return +0.;
    }
#pragma GCC diagnostic pop

    return (x < 0.) ? -x : x;
}

template <typename T, std::size_t N>
auto fabs(const std::array<T, N> &a) {
    std::array<T, N> res;

    for (std::size_t i = 0; i < N; ++i) {
        res[i] = fabs(a[i]);
    }

    return res;
}

template <typename T>
auto fabs(const std::vector<T> &v) {
    std::vector<T> res(v.size());

    for (std::size_t i = 0; i < v.size(); ++i) {
        res[i] = fabs(v[i]);
    }

    return res;
}

} // namespace maths
} // namespace scicpp

#endif // SCICPP_MATHS_MATHS
