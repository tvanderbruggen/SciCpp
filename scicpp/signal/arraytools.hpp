#ifndef SCICPP_SIGNAL_ARRAYTOOLS
#define SCICPP_SIGNAL_ARRAYTOOLS

#include "scicpp/core/macros.hpp"
#include "scicpp/core/manips.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/units/quantity.hpp"

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

// ----------------------------------------------------------------------------
// Boundary extension functions
// ----------------------------------------------------------------------------

template <typename Array, typename DiffTp = typename Array::difference_type>
auto odd_ext(const Array &x, DiffTp n) {
    using T = typename Array::value_type;
    using raw_t = units::representation_t<T>;
    using namespace scicpp::operators;

    const auto size = DiffTp(x.size());
    scicpp_require(n <= size - 1);

    if (n < 1) {
        return std::vector(x.cbegin(), x.cend());
    }

    const auto left_end = x[0] * raw_t(2);
    auto left_ext = slice_array(x, n, 0, -1);
    const auto right_end = x.back() * raw_t(2);
    auto right_ext = slice_array(x, -2, -(n + 2), -1);

    return (left_end - left_ext) | x | (right_end - right_ext);
}

template <typename Array, typename DiffTp = typename Array::difference_type>
auto even_ext(const Array &x, DiffTp n) {
    using namespace scicpp::operators;

    scicpp_require(n <= DiffTp(x.size()) - 1);

    if (n < 1) {
        return std::vector(x.cbegin(), x.cend());
    }

    return slice_array(x, n, 0, -1) | x | slice_array(x, -2, -(n + 2), -1);
}

template <typename Array, typename DiffTp = typename Array::difference_type>
auto const_ext(const Array &x, DiffTp n) {
    using T = typename Array::value_type;
    using raw_t = units::representation_t<T>;
    using namespace scicpp::operators;

    scicpp_require(!x.empty());

    if (n < 1) {
        return std::vector(x.cbegin(), x.cend());
    }

    return (ones<raw_t>(std::size_t(n)) * x[0]) | x |
           (ones<raw_t>(std::size_t(n)) * x.back());
}

template <typename Array, typename DiffTp = typename Array::difference_type>
auto zero_ext(const Array &x, DiffTp n) {
    using T = typename Array::value_type;
    using namespace scicpp::operators;

    if (n < 1) {
        return std::vector(x.cbegin(), x.cend());
    }

    return zeros<T>(std::size_t(n)) | x | zeros<T>(std::size_t(n));
}

} // namespace scicpp::signal

#endif
