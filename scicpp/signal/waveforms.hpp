// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_SIGNAL_WAVEFORMS
#define SCICPP_SIGNAL_WAVEFORMS

#include "scicpp/core/macros.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>

namespace scicpp::signal {

//---------------------------------------------------------------------------------
// unit_impulse
//---------------------------------------------------------------------------------

template <typename T, std::size_t N>
constexpr auto unit_impulse(std::size_t idx = 0) {
    std::array<T, N> res{};

    if (idx < N) {
        res[idx] = T(1);
    }

    return res;
}

template <typename T>
auto unit_impulse(std::size_t len, std::size_t idx = 0) {
    std::vector<T> res(len, T(0));

    if (idx < len) {
        res[idx] = T(1);
    }

    return res;
}

//---------------------------------------------------------------------------------
// sawtooth
//---------------------------------------------------------------------------------

namespace detail {

template <class Array, typename T = typename Array::value_type>
void sawtooth_filler(Array &res, const Array &t, T width) {
    SCICPP_REQUIRE(width >= T(0) && width <= T(1));

    std::transform(t.cbegin(), t.cend(), res.begin(), [=](auto t_) {
        const auto tmod = std::fmod(t_, T(2) * M_PI);

        if (tmod < width * T(2) * M_PI) {
            SCICPP_REQUIRE(width > T(0));
            return tmod / (width * M_PI) - T(1);
        } else {
            SCICPP_REQUIRE(width < T(1));
            return M_1_PI * (M_PI * (width + T(1)) - tmod) / (T(1) - width);
        }
    });
}

} // namespace detail

template <typename T, std::size_t N>
auto sawtooth(const std::array<T, N> &t, T width = T(1)) {
    std::array<T, N> res{};
    detail::sawtooth_filler(res, t, width);
    return res;
}

template <typename T>
auto sawtooth(const std::vector<T> &t, T width = T(1)) {
    std::vector<T> res(t.size());
    detail::sawtooth_filler(res, t, width);
    return res;
}

} // namespace scicpp::signal

#endif // SCICPP_SIGNAL_WAVEFORMS