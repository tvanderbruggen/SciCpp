// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_PRINT
#define SCICPP_CORE_PRINT

#include "scicpp/core/meta.hpp"

#include <array>
#include <cstdio>
#include <string>
#include <vector>

namespace scicpp {

namespace detail {

// https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
inline void
replace_all(std::string &str, const std::string &from, const std::string &to) {
    if (from.empty()) {
        return;
    }

    std::size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos +=
            to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

} // namespace detail

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"

template <typename T>
void fprint(FILE *stream, std::string fmt, T value) {
    if constexpr (meta::is_complex_v<T>) {
        using scalar_t = typename T::value_type;

        if (value.imag() >= scalar_t(0)) {
            detail::replace_all(fmt, "%f", "%f + %fi");
            std::fprintf(stream, fmt.c_str(), value.real(), value.imag());
        } else {
            detail::replace_all(fmt, "%f", "%f - %fi");
            std::fprintf(stream, fmt.c_str(), value.real(), -value.imag());
        }
    } else {
        std::fprintf(stream, fmt.c_str(), value);
    }
}

#pragma GCC diagnostic pop

template <std::size_t n_max,     // Max size for untruncated display
          std::size_t n_cols,    // Number of printed columns
          std::size_t n_display, // Number of display points when truncated
          class Array>
void fprint(FILE *stream, const Array &A) {
    std::fprintf(stream, "[ ");

    if (A.size() <= n_max) {
        for (std::size_t i = 0; i < A.size() - 1; ++i) {
            fprint(stream, "%f, ", A[i]);

            if ((i > 0) && (i % n_cols == 0)) {
                std::fprintf(stream, "\n  ");
            }
        }
    } else {
        for (std::size_t i = 0; i < n_display; ++i) {
            fprint(stream, "%f, ", A[i]);
        }

        std::fprintf(stream, "..., ");

        for (std::size_t i = A.size() - n_display; i < A.size() - 1; ++i) {
            fprint(stream, "%f, ", A[i]);
        }
    }

    fprint(stream, "%f ]\n", A[A.size() - 1]);
}

// Specialization for default n_max, n_cols and n_display values
template <class Array>
void fprint(FILE *stream, const Array &A) {
    return fprint<1000, 5, 3>(stream, A);
}

template <class Array>
void print(const Array &A) {
    return fprint(stdout, A);
}

} // namespace scicpp

#endif // SCICPP_CORE_PRINT