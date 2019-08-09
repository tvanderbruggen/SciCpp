// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_PRINT
#define SCICPP_CORE_PRINT

#include "scicpp/core/meta.hpp"
#include "scicpp/core/units.hpp"

#include <array>
#include <cstdio>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace scicpp {

namespace detail {

template <typename T>
void fprint_element(FILE *stream, T value);

template <typename T>
constexpr auto type_to_format() {
    if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) {
        return "%f";
    } else if constexpr (std::is_same_v<T, long double>) {
        return "%Lf";
    } else if constexpr (std::is_same_v<T, long unsigned int>) {
        return "%lu";
    } else if constexpr (std::is_same_v<T, unsigned int>) {
        return "%u";
    } else if constexpr (std::is_same_v<T, long int>) {
        return "%li";
    } else if constexpr (std::is_same_v<T, int>) {
        return "%i";
    } else if constexpr (std::is_same_v<T, bool>) {
        return "%u";
    }
}

template <typename T>
void fprint_tuple_element(FILE *stream, T x) {
    fprint_element(stream, x);
    std::fprintf(stream, ", ");
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"

template <typename T>
void fprint_element(FILE *stream, T value) {
    if constexpr (meta::is_complex_v<T>) {
        using scalar_t = typename T::value_type;

        auto fmt = std::string(type_to_format<scalar_t>());

        if (value.imag() >= scalar_t(0)) {
            auto fmt_ = fmt + " + " + fmt + "i";
            std::fprintf(stream, fmt_.c_str(), value.real(), value.imag());
        } else {
            auto fmt_ = fmt + " - " + fmt + "i";
            std::fprintf(stream, fmt_.c_str(), value.real(), -value.imag());
        }
    } else if constexpr (meta::is_std_tuple_v<T> || meta::is_std_pair_v<T>) {
        std::fprintf(stream, "(");
        std::apply(
            [stream](auto... x) { (fprint_tuple_element(stream, x), ...); },
            meta::subtuple<1>(value));
        fprint_element(stream, std::get<std::tuple_size_v<T> - 1>(value));
        std::fprintf(stream, ")");
    } else if constexpr (units::is_quantity_v<T>) {
        using raw_t = typename T::value_type;
        std::fprintf(stream, type_to_format<raw_t>(), value.value());
    } else {
        std::fprintf(stream, type_to_format<T>(), value);
    }
}

#pragma GCC diagnostic pop

} // namespace detail

template <std::size_t n_max,     // Max size for untruncated display
          std::size_t n_cols,    // Number of printed columns
          std::size_t n_display, // Number of display points when truncated
          class Array>
void fprint(FILE *stream, const Array &A) {
    static_assert(n_cols != 0);

    std::fprintf(stream, "[ ");

    if (A.size() <= n_max) {
        for (std::size_t i = 0; i < A.size() - 1; ++i) {
            detail::fprint_element(stream, A[i]);
            std::fprintf(stream, ", ");

            if constexpr (n_cols == 1) {
                std::fprintf(stream, "\n  ");
            } else {
                if ((i > 0) && ((i + 1) % n_cols == 0)) {
                    std::fprintf(stream, "\n  ");
                }
            }
        }
    } else {
        for (std::size_t i = 0; i < n_display; ++i) {
            detail::fprint_element(stream, A[i]);
            std::fprintf(stream, ", ");
        }

        std::fprintf(stream, "..., ");

        for (std::size_t i = A.size() - n_display; i < A.size() - 1; ++i) {
            detail::fprint_element(stream, A[i]);
            std::fprintf(stream, ", ");
        }
    }

    detail::fprint_element(stream, A[A.size() - 1]);
    std::fprintf(stream, " ]\n");
}

// Specialization for default n_max, n_cols and n_display values
template <class Array>
void fprint(FILE *stream, const Array &A) {
    if constexpr (meta::is_std_tuple_v<typename Array::value_type>) {
        return fprint<1000, 1, 3>(stream, A);
    } else {
        return fprint<1000, 5, 3>(stream, A);
    }
}

template <class Array>
void print(const Array &A) {
    return fprint(stdout, A);
}

} // namespace scicpp

#endif // SCICPP_CORE_PRINT