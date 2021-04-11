// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_PRINT
#define SCICPP_CORE_PRINT

#include "scicpp/core/equal.hpp"
#include "scicpp/core/functional.hpp"
#include "scicpp/core/maths.hpp"
#include "scicpp/core/meta.hpp"
#include "scicpp/core/stats.hpp"
#include "scicpp/core/units/quantity.hpp"

#include <array>
#include <complex>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace scicpp {

namespace detail {

template <typename T>
auto fprint_element(T value, int precision, bool fixed, int width);

template <class Stream, typename T>
void fprint_tuple_element(
    Stream &stream, T x, int precision, bool fixed, int width) {
    stream << fprint_element(x, precision, fixed, width);
    stream << ", ";
}

template <typename T>
bool is_negative(T value) {
    return (std::fpclassify(value) == FP_ZERO &&
            std::signbit(value)) // Negative zero
           || value < T(0);
}

template <class Stream, typename T>
void fprint_complex(Stream &stream, const std::complex<T> &z) {
    if (is_negative(z.imag())) {
        stream << z.real() << "-" << -z.imag() << "j";
    } else {
        stream << z.real() << "+" << z.imag() << "j";
    }
}

template <typename T>
auto fprint_element(T value, int precision, bool fixed, int width) {
    std::stringstream ss;

    if (fixed) {
        ss << std::fixed;
    } else {
        ss << std::scientific;
    }

    ss << std::setprecision(precision);

    if (width >= 0) {
        ss << std::setw(width);
    }

    ss << std::setfill(' ');

    if constexpr (meta::is_complex_v<T>) {
        if constexpr (units::is_quantity_v<typename T::value_type>) {
            fprint_complex(ss, units::value(value));
        } else {
            fprint_complex(ss, value);
        }
    } else if constexpr (meta::is_std_tuple_v<T> || meta::is_std_pair_v<T>) {
        ss << "(";
        std::apply(
            [&](auto... x) {
                (fprint_tuple_element(ss, x, precision, fixed, width), ...);
            },
            meta::subtuple<1>(value));
        ss << fprint_element(
            std::get<std::tuple_size_v<T> - 1>(value), precision, fixed, width);
        ss << ")";
    } else if constexpr (units::is_quantity_v<T>) {
        ss << value.value();
    } else {
        ss << value;
    }

    return ss.str();
}

template <class Array>
int get_width([[maybe_unused]] const Array &A, int fixed) {
    if constexpr (meta::is_std_tuple_v<typename Array::value_type>) {
        return -1;
    }

    if (!fixed) {
        return 15;
    } else {
        return 12;
    }
}

template <class Array>
bool use_scientific_notation(const Array &A) {
    // https://numpy.org/doc/stable/reference/generated/numpy.set_printoptions.html
    // Scientific notation is used when absolute value of the smallest number is < 1e-4
    // or the ratio of the maximum absolute value to the minimum is > 1e3.

    using T = typename Array::value_type;

    if constexpr (meta::is_complex_v<T>) {
        return use_scientific_notation(real(A)) ||
               use_scientific_notation(imag(A));
    } else if constexpr (meta::is_std_tuple_v<T> || meta::is_std_pair_v<T>) {
        return true;
    } else {
        const auto fabs = vectorize([](auto x) { return units::fabs(x); });
        const auto abs_non_zero = filter(fabs(A), filters::not_zero);

        if (abs_non_zero.size() == 0) {
            return false;
        }

        const auto min = units::value(stats::amin(abs_non_zero));
        const auto max = units::value(stats::amax(abs_non_zero));
        return (min < 1E-4) || ((max / min) > 1E3);
    }
}

template <class Array>
auto get_edgeitems(const Array &A, std::size_t num_edgeitems) {
    std::vector<typename Array::value_type> edgeitems;
    edgeitems.reserve(2 * num_edgeitems);

    for (std::size_t i = 0; i < num_edgeitems; ++i) {
        edgeitems.push_back(A[i]);
    }

    for (std::size_t i = A.size() - num_edgeitems; i < A.size(); ++i) {
        edgeitems.push_back(A[i]);
    }

    return edgeitems;
}

} // namespace detail

struct PrintOptions {
    PrintOptions()
        : separator(' '), precision(8), threshold(1000), linewidth(75),
          edgeitems(3) {}

    char separator;
    int precision;
    // Total number of array elements which trigger summarization rather than full repr
    std::size_t threshold;
    // Number of characters per line for the purpose of inserting line breaks
    int linewidth;
    // Number of array items in summary at beginning and end of each dimension
    std::size_t edgeitems;
};

template <class Stream, class Array, meta::enable_if_iterable<Array> = 0>
void fprint(Stream &stream, const Array &A, const PrintOptions &prtopts) {
    stream << "[";
    int pos = 1;

    if (A.size() <= prtopts.threshold) {
        const auto fixed = !detail::use_scientific_notation(A);
        const auto width = detail::get_width(A, fixed);

        for (std::size_t i = 0; i < A.size() - 1; ++i) {
            const auto str = detail::fprint_element(
                A[i], prtopts.precision, fixed, width);
            stream << prtopts.separator;
            pos += (1 + int(str.size()));

            if (pos >= prtopts.linewidth) {
                stream << "\n  " << str;
                pos = 2 + int(str.size());
            } else {
                stream << str;
            }
        }

        stream << prtopts.separator;
        stream << detail::fprint_element(
            A[A.size() - 1], prtopts.precision, fixed, width);
    } else {
        const auto fixed = !detail::use_scientific_notation(
            detail::get_edgeitems(A, prtopts.edgeitems));
        const auto width = detail::get_width(A, fixed);

        for (std::size_t i = 0; i < prtopts.edgeitems; ++i) {
            const auto str = detail::fprint_element(
                A[i], prtopts.precision, fixed, width);
            stream << prtopts.separator;
            pos += (1 + int(str.size()));

            if (pos >= prtopts.linewidth) {
                stream << "\n  " << str;
                pos = 2 + int(str.size());
            } else {
                stream << str;
            }
        }

        stream << prtopts.separator << "...";
        pos += 3;

        for (std::size_t i = A.size() - prtopts.edgeitems;
             i < A.size() - 1;
             ++i) {
            const auto str = detail::fprint_element(
                A[i], prtopts.precision, fixed, width);
            stream << prtopts.separator;
            pos += (1 + int(str.size()));

            if (pos >= prtopts.linewidth) {
                stream << "\n  " << str;
                pos = 2 + int(str.size());
            } else {
                stream << str;
            }
        }

        stream << detail::fprint_element(
            A[A.size() - 1], prtopts.precision, fixed, width);
    }

    stream << "]\n";
}

template <class Stream, class T>
void fprint(Stream &stream, const T &A) {
    if constexpr (meta::is_iterable_v<T>) {
        fprint(stream, A, PrintOptions{});
    } else {
        stream << detail::fprint_element(
            A, PrintOptions{}.precision, false, 12);
        stream << "\n";
    }
}

template <class T>
void print(const T &A) {
    fprint(std::cout, A);
}

} // namespace scicpp

#endif // SCICPP_CORE_PRINT