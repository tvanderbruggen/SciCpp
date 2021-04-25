// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_SIGNAL_WINDOWS
#define SCICPP_SIGNAL_WINDOWS

#include "scicpp/core/constants.hpp"
#include "scicpp/core/macros.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <vector>

namespace scicpp::signal::windows {

namespace detail {

// Window functions are symmetric.
// So we compute only one half and mirror it to the upper
// region of the vector.

template <class Array, typename Func>
void symmetric_filler(Array &w, Func f) {
    const auto half_len = signed_size_t(w.size()) / 2;
    std::iota(w.begin() + half_len, w.end(), half_len);
    std::transform(w.cbegin() + half_len, w.cend(), w.begin() + half_len, f);
    std::reverse_copy(w.cbegin() + half_len, w.cend(), w.begin());
}

} // namespace detail

//---------------------------------------------------------------------------------
// Polynomial windows
//---------------------------------------------------------------------------------

template <typename T, std::size_t M>
auto boxcar() {
    std::array<T, M> w{};
    w.fill(T{1});
    return w;
}

template <typename T>
auto boxcar(std::size_t M) {
    return std::vector<T>(M, T{1});
}

namespace detail {

template <class Array>
constexpr void bartlett_filler(Array &w) {
    using T = typename Array::value_type;
    const auto scaling = -T{2} / T(w.size() - 1);
    symmetric_filler(w, [&](auto i) { return std::fma(scaling, T(i), T{2}); });
}

} // namespace detail

template <typename T, std::size_t M>
constexpr auto bartlett() {
    static_assert(M > 1);
    std::array<T, M> w{};
    detail::bartlett_filler(w);
    return w;
}

template <typename T>
auto bartlett(std::size_t M) {
    scicpp_require(M > 1);
    std::vector<T> w(M);
    detail::bartlett_filler(w);
    return w;
}

//---------------------------------------------------------------------------------
// Cosine window
//---------------------------------------------------------------------------------

namespace detail {

template <class Array>
void cosine_filler(Array &w) {
    if (w.size() > 0) {
        using T = typename Array::value_type;
        const T scaling = pi<T> / T(w.size());
        symmetric_filler(
            w, [&](std::size_t i) { return std::sin(scaling * (T(i) + 0.5)); });
    }
}

} // namespace detail

template <typename T, std::size_t M>
auto cosine() {
    std::array<T, M> w{};
    detail::cosine_filler(w);
    return w;
}

template <typename T>
auto cosine(std::size_t M) {
    std::vector<T> w(M);
    detail::cosine_filler(w);
    return w;
}

//---------------------------------------------------------------------------------
// Cosine sum windows
//---------------------------------------------------------------------------------

namespace detail {

template <class Array,
          std::size_t n_weights,
          typename T = typename Array::value_type>
void general_cosine(Array &w, const std::array<T, n_weights> &a) {
    const auto scaling = T{2} * pi<T> / T(w.size() - 1);

    symmetric_filler(w, [&](std::size_t i) {
        auto tmp = T{0};
        auto sign = -T{1};
        std::size_t j = 0;

        for (const auto &c : a) {
            sign *= -T{1};
            tmp += sign * c * std::cos(scaling * T(i * j));
            j++;
        }

        return tmp;
    });
}

} // namespace detail

template <typename T, std::size_t n_weights>
auto general_cosine(std::size_t M, const std::array<T, n_weights> &a) {
    scicpp_require(M > 1);
    std::vector<T> w(M);
    detail::general_cosine(w, a);
    return w;
}

template <typename T, std::size_t M, std::size_t n_weights>
auto general_cosine(const std::array<T, n_weights> &a) {
    static_assert(M > 1);
    std::array<T, M> w{};
    detail::general_cosine(w, a);
    return w;
}

template <typename T, std::size_t M>
auto general_hamming(T alpha) {
    return general_cosine<T, M>(std::array{alpha, T{1} - alpha});
}

template <typename T>
auto general_hamming(std::size_t M, T alpha) {
    return general_cosine(M, std::array{alpha, T{1} - alpha});
}

template <typename T, std::size_t M>
auto hann() {
    return general_hamming<T, M>(T{0.5});
}

template <typename T>
auto hann(std::size_t M) {
    return general_hamming(M, T{0.5});
}

template <typename T, std::size_t M>
auto hamming() {
    return general_hamming<T, M>(T{0.54});
}

template <typename T>
auto hamming(std::size_t M) {
    return general_hamming(M, T{0.54});
}

template <typename T, std::size_t M>
auto blackman() {
    constexpr std::array a{0.42, 0.50, 0.08};
    return general_cosine<T, M>(a);
}

template <typename T>
auto blackman(std::size_t M) {
    constexpr std::array a{0.42, 0.50, 0.08};
    return general_cosine(M, a);
}

template <typename T, std::size_t M>
auto nuttall() {
    constexpr std::array a{0.3635819, 0.4891775, 0.1365995, 0.0106411};
    return general_cosine<T, M>(a);
}

template <typename T>
auto nuttall(std::size_t M) {
    constexpr std::array a{0.3635819, 0.4891775, 0.1365995, 0.0106411};
    return general_cosine(M, a);
}

template <typename T, std::size_t M>
auto blackmanharris() {
    constexpr std::array a{0.35875, 0.48829, 0.14128, 0.01168};
    return general_cosine<T, M>(a);
}

template <typename T>
auto blackmanharris(std::size_t M) {
    constexpr std::array a{0.35875, 0.48829, 0.14128, 0.01168};
    return general_cosine(M, a);
}

template <typename T, std::size_t M>
auto flattop() {
    constexpr std::array a{
        0.21557895, 0.41663158, 0.277263158, 0.083578947, 0.006947368};
    return general_cosine<T, M>(a);
}

template <typename T>
auto flattop(std::size_t M) {
    constexpr std::array a{
        0.21557895, 0.41663158, 0.277263158, 0.083578947, 0.006947368};
    return general_cosine(M, a);
}

//---------------------------------------------------------------------------------
// Gaussian
//---------------------------------------------------------------------------------

namespace detail {

template <class Array, typename T = typename Array::value_type>
void gaussian_filler(Array &w, T sigma) {
    scicpp_require(sigma > T{0});

    const T shift = w.size() % 2 == 0 ? T{0.5} : T{0};
    const T i0 = T(w.size() / 2) - shift;
    const T scaling = -T{1} / (T{2} * sigma * sigma);

    symmetric_filler(w, [=](std::size_t i) {
        const T n = T(i) - i0;
        return std::exp(scaling * n * n);
    });
}

} // namespace detail

template <typename T, std::size_t M>
auto gaussian(T sigma) {
    std::array<T, M> w{};
    detail::gaussian_filler(w, sigma);
    return w;
}

template <typename T>
auto gaussian(std::size_t M, T sigma) {
    std::vector<T> w(M);
    detail::gaussian_filler(w, sigma);
    return w;
}

//---------------------------------------------------------------------------------
// get_window
//---------------------------------------------------------------------------------

enum Window : std::size_t {
    Boxcar,
    Bartlett,
    Cosine,
    Hann,
    Hamming,
    Blackman,
    Nuttall,
    Blackmanharris,
    Flattop
};

template <Window win, std::size_t N, typename T = double>
auto get_window() {
    switch (win) {
    case Boxcar:
        return boxcar<T, N>();
    case Bartlett:
        return bartlett<T, N>();
    case Cosine:
        return cosine<T, N>();
    case Hann:
        return hann<T, N>();
    case Hamming:
        return hamming<T, N>();
    case Blackman:
        return blackman<T, N>();
    case Nuttall:
        return nuttall<T, N>();
    case Blackmanharris:
        return blackmanharris<T, N>();
    case Flattop:
        return flattop<T, N>();
    default:
        scicpp_unreachable;
    }
}

template <typename T = double>
auto get_window(Window win, std::size_t N) {
    switch (win) {
    case Boxcar:
        return boxcar<T>(N);
    case Bartlett:
        return bartlett<T>(N);
    case Cosine:
        return cosine<T>(N);
    case Hann:
        return hann<T>(N);
    case Hamming:
        return hamming<T>(N);
    case Blackman:
        return blackman<T>(N);
    case Nuttall:
        return nuttall<T>(N);
    case Blackmanharris:
        return blackmanharris<T>(N);
    case Flattop:
        return flattop<T>(N);
    default:
        scicpp_unreachable;
    }
}

} // namespace scicpp::signal::windows

#endif // SCICPP_SIGNAL_WINDOWS
