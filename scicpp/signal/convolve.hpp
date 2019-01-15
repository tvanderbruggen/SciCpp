// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_SIGNAL_CONVOLVE
#define SCICPP_SIGNAL_CONVOLVE

#include "scicpp/core/macros.hpp"
#include "scicpp/core/meta.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/signal/fft.hpp"

#include <algorithm>
#include <array>
#include <complex>
#include <type_traits>
#include <vector>

namespace scicpp::signal {

enum ConvMethod : int { DIRECT, FFT };

//---------------------------------------------------------------------------------
// direct_convolve
//---------------------------------------------------------------------------------

namespace detail {

// For now only the "full" mode is implemented
// https://stackoverflow.com/questions/24518989/how-to-perform-1-dimensional-valid-convolution
template <class U, class V, class W>
constexpr void direct_convolve_impl(U &res, const V &a, const W &v) {
    static_assert(
        std::is_same_v<typename U::value_type, typename V::value_type>);
    static_assert(
        std::is_same_v<typename U::value_type, typename W::value_type>);

    const int n = int(a.size());
    const int m = int(v.size());
    SCICPP_REQUIRE(int(res.size()) == n + m - 1);

    for (int i = 0; i < n + m - 1; ++i) {
        res[std::size_t(i)] = 0.0;
        const int jmn = (i >= m - 1) ? i - (m - 1) : 0;
        const int jmx = (i < n - 1) ? i : n - 1;
        for (auto j = jmn; j <= jmx; ++j) {
            res[std::size_t(i)] += (a[std::size_t(j)] * v[std::size_t(i - j)]);
        }
    }
}

template <typename T, std::size_t N, std::size_t M>
constexpr auto direct_convolve(const std::array<T, N> &a,
                               const std::array<T, M> &v) {
    std::array<T, N + M - 1> res{};

    if constexpr (M <= N) {
        detail::direct_convolve_impl(res, a, v);
    } else {
        detail::direct_convolve_impl(res, v, a);
    }

    return res;
}

template <typename T>
auto direct_convolve(const std::vector<T> &a, const std::vector<T> &v) {
    std::vector<T> res(a.size() + v.size() - 1);

    // Same behavior as numpy:
    // If v is longer than a, the arrays are swapped before computation.
    if (v.size() <= a.size()) {
        detail::direct_convolve_impl(res, a, v);
    } else {
        detail::direct_convolve_impl(res, v, a);
    }

    return res;
}

} // namespace detail

//---------------------------------------------------------------------------------
// fftconvolve
//---------------------------------------------------------------------------------

template <typename T>
auto fftconvolve(const std::vector<T> &a, const std::vector<T> &v) {
    const auto res_size = a.size() + v.size() - 1;
    const auto fft_size = next_fast_len(res_size);

    if constexpr (meta::is_complex_v<T>) {
        const auto f_a = fft(zero_padding(a, fft_size));
        const auto f_v = fft(zero_padding(v, fft_size));
        const auto res = ifft(f_v * f_a, int(fft_size));

        return std::vector<T>(
            std::make_move_iterator(res.begin()),
            std::make_move_iterator(res.begin() + int(res_size)));
    } else {
        const auto f_a = rfft(zero_padding(a, fft_size));
        const auto f_v = rfft(zero_padding(v, fft_size));
        const auto res = irfft(f_v * f_a, int(fft_size));

        return std::vector<T>(
            std::make_move_iterator(res.begin()),
            std::make_move_iterator(res.begin() + int(res_size)));
    }
}

//---------------------------------------------------------------------------------
// convolve
//---------------------------------------------------------------------------------

template <ConvMethod method, class U, class V>
constexpr auto convolve(const U &a, const V &v) {
    static_assert(
        std::is_same_v<typename U::value_type, typename V::value_type>);

    if constexpr (method == DIRECT) {
        return detail::direct_convolve(a, v);
    } else {
        return fftconvolve(a, v);
    }
}

template <class U, class V>
constexpr auto convolve(const U &a, const V &v) {
    return convolve<DIRECT>(a, v);
}

//---------------------------------------------------------------------------------
// correlate
//---------------------------------------------------------------------------------

namespace detail {

template <class Array>
void reverse_conj(const Array &a, Array &res) {
    std::reverse_copy(a.cbegin(), a.cend(), res.begin());

    if constexpr (meta::is_complex_v<typename Array::value_type>) {
        std::transform(res.cbegin(), res.cend(), res.begin(), [](auto v) {
            return std::conj(v);
        });
    }
}

} // namespace detail

template <ConvMethod method, typename T, std::size_t N, std::size_t M>
constexpr auto correlate(const std::array<T, N> &a, const std::array<T, M> &v) {
    std::array<T, M> v_rev{};
    detail::reverse_conj(v, v_rev);
    return convolve<method>(a, v_rev);
}

template <ConvMethod method, typename T>
auto correlate(const std::vector<T> &a, const std::vector<T> &v) {
    std::vector<T> v_rev(v.size());
    detail::reverse_conj(v, v_rev);
    return convolve<method>(a, v_rev);
}

template <class U, class V>
constexpr auto correlate(const U &a, const V &v) {
    return correlate<DIRECT>(a, v);
}

} // namespace scicpp::signal

#endif // SCICPP_SIGNAL_CONVOLVE