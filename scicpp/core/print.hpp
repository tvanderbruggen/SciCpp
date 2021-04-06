// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_PRINT
#define SCICPP_CORE_PRINT

#include "scicpp/core/equal.hpp"
#include "scicpp/core/meta.hpp"
#include "scicpp/core/units/quantity.hpp"

#include <array>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace scicpp {

namespace detail {

template <class Stream, typename T>
void fprint_element(Stream &stream, T value);

template <class Stream, typename T>
void fprint_tuple_element(Stream &stream, T x) {
    fprint_element(stream, x);
    stream << ", ";
}

template <class Stream, typename T>
void fprint_complex(Stream &stream, T z) {
    using scalar_t = typename T::value_type;

    if (std::fpclassify(z.imag()) == FP_ZERO &&
        std::signbit(z.imag())) { // Negative zero
        stream << z.real() << " - " << -z.imag() << "i";
    } else if (z.imag() >= scalar_t(0)) {
        stream << z.real() << " + " << z.imag() << "i";
    } else {
        stream << z.real() << " - " << -z.imag() << "i";
    }
}

template <class Stream, typename T>
void fprint_element(Stream &stream, T value) {
    if constexpr (meta::is_complex_v<T>) {
        if constexpr (units::is_quantity_v<typename T::value_type>) {
            fprint_complex(stream, units::value(value));
        } else {
            fprint_complex(stream, value);
        }
    } else if constexpr (meta::is_std_tuple_v<T> || meta::is_std_pair_v<T>) {
        stream << "(";
        std::apply(
            [&stream](auto... x) { (fprint_tuple_element(stream, x), ...); },
            meta::subtuple<1>(value));
        fprint_element(stream, std::get<std::tuple_size_v<T> - 1>(value));
        stream << ")";
    } else if constexpr (units::is_quantity_v<T>) {
        stream << value.value();
    } else {
        stream << value;
    }
}

} // namespace detail

template <std::size_t n_max,     // Max size for untruncated display
          std::size_t n_cols,    // Number of printed columns
          std::size_t n_display, // Number of display points when truncated
          class Stream,
          class Array,
          meta::enable_if_iterable<Array> = 0>
void fprint(Stream &stream, const Array &A) {
    static_assert(n_cols != 0);

    stream << "[ ";

    if (A.size() <= n_max) {
        for (std::size_t i = 0; i < A.size() - 1; ++i) {
            detail::fprint_element(stream, A[i]);
            stream << ", ";

            if constexpr (n_cols == 1) {
                stream << "\n  ";
            } else {
                if ((i > 0) && ((i + 1) % n_cols == 0)) {
                    stream << "\n  ";
                }
            }
        }
    } else {
        for (std::size_t i = 0; i < n_display; ++i) {
            detail::fprint_element(stream, A[i]);
            stream << ", ";
        }

        stream << "..., ";

        for (std::size_t i = A.size() - n_display; i < A.size() - 1; ++i) {
            detail::fprint_element(stream, A[i]);
            stream << ", ";
        }
    }

    detail::fprint_element(stream, A[A.size() - 1]);
    stream << " ]\n";
}

// Specialization for default n_max, n_cols and n_display values
template <class Stream, class T>
void fprint(Stream &stream, const T &A) {
    if constexpr (meta::is_iterable_v<T>) {
        if constexpr (meta::is_std_tuple_v<typename T::value_type>) {
            fprint<1000, 1, 3>(stream, A);
        } else {
            fprint<1000, 5, 3>(stream, A);
        }
    } else {
        detail::fprint_element(stream, A);
        stream << "\n";
    }
}

template <class T>
void print(const T &A) {
    fprint(std::cout, A);
}

} // namespace scicpp

#endif // SCICPP_CORE_PRINT