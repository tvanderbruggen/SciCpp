#ifndef SCICPP_SIGNAL_ARRAYTOOLS
#define SCICPP_SIGNAL_ARRAYTOOLS

#include "scicpp/core/macros.hpp"
#include "scicpp/core/manips.hpp"

#include <vector>

namespace scicpp::signal {

namespace detail {

// template <typename T>
// auto eye(size_t N, size_t M = 0, int k = 0) {
//     if (M == 0)
//         M = N;
//     std::vector<std::vector<T>> result;
//     for (size_t i = 0; i < N; ++i) {
//         std::vector<T> row = zeros<T>(M);
//         if ((i + k >= 0) && (i + k < N))
//             row[i + k] = 1;
//         result.push_back(row);
//     }
//     return result;
// }
} // namespace detail

template <typename T>
auto axis_slice(const std::vector<T> &arr,
                signed_size_t start,
                signed_size_t stop,
                signed_size_t step = 1) {
    return slice_array(arr, start, stop, step);
}

template <typename T>
auto odd_ext(const std::vector<T> &x, signed_size_t n) {
    using namespace scicpp::operators;

    scicpp_require(n <= signed_size_t(x.size() - 1));

    if (n < 1) {
        return x;
    }

    auto left_end = axis_slice(x, 0, 1)[0] * 2;
    auto left_ext = axis_slice(x, n, 0, -1);
    auto right_end = axis_slice(x, -1, signed_size_t(x.size()))[0] * 2;
    auto right_ext = axis_slice(x, -2, -(n + 2), -1);

    return (left_end - left_ext) | x | (right_end - right_ext);
}

} // namespace scicpp::signal

#endif
