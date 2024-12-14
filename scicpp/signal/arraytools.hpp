#ifndef SCICPP_SIGNAL_ARRAYTOOLS
#define SCICPP_SIGNAL_ARRAYTOOLS

#include "scicpp/core/macros.hpp"
#include "scicpp/core/manips.hpp"

#include <vector>

namespace scicpp::signal {
using namespace std;
namespace detail {

template <typename T>
auto slice(const std::vector<T> &arr,
           signed_size_t start,
           signed_size_t stop,
           signed_size_t step = 1) {
    std::vector<T> res;
    const auto n = static_cast<signed_size_t>(arr.size());

    start = start < 0 ? n + start : start;
    stop = stop < 0 ? n + stop : stop;

    auto left = std::min(start, stop);
    auto right = std::max(start, stop);

    if (left < 0) {
        left = -1;
    }

    if (right > n) {
        right = n;
    }

    while (right > left) {
        if ((step < 0) && (stop < start)) {
            res.push_back(arr[static_cast<std::size_t>(right)]);
            right += step;
        } else if ((step > 0) && (stop > start)) {
            res.push_back(arr[static_cast<std::size_t>(left)]);
            left += step;
        } else {
            break;
        }
    }

    return res;
}


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
    return detail::slice(arr, start, stop, step);
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
