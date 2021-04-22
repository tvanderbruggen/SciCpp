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
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace scicpp {

namespace detail {

struct ElementFormater {
    ElementFormater(int precision, bool fixed, int width)
        : precision(precision), fixed(fixed), width(width) {
        if (fixed) {
            ss << std::fixed;
        } else {
            ss << std::scientific;
        }

        ss << std::setprecision(precision);
        ss << std::setfill(' ');
    }

    void reset() {
        // https://stackoverflow.com/questions/7623650/resetting-a-stringstream
        ss.str(std::string());
        ss.clear();

        if (width >= 0) {
            ss << std::setw(width);
        }
    }

    template <typename T>
    void print_tuple_element(T x) {
        print_element(x, false);
        ss << ", ";
    }

    template <typename T>
    bool is_negative(T value) const {
        return (std::fpclassify(value) == FP_ZERO &&
                std::signbit(value)) // Negative zero
               || value < T(0);
    }

    template <typename T>
    void print_complex(const std::complex<T> &z) {
        if (is_negative(z.imag())) {
            ss << z.real() << "-" << -z.imag() << "j";
        } else {
            ss << z.real() << "+" << z.imag() << "j";
        }
    }

    template <typename T>
    auto print_element(T value, bool rst = true) {
        if (rst) {
            reset();
        }

        if constexpr (meta::is_complex_v<T>) {
            print_complex(units::value(value));
        } else if constexpr (meta::is_std_tuple_v<T> ||
                             meta::is_std_pair_v<T>) {
            ss << "(";
            std::apply([&](auto... x) { (print_tuple_element(x), ...); },
                       meta::subtuple<1>(value));
            print_element(std::get<std::tuple_size_v<T> - 1>(value), false);
            ss << ")";
        } else {
            ss << units::value(value);
        }
    }

    auto remove_trailling_zeros(std::string &str) const {
        if (str != "0") {
            const auto first = str.find_last_not_of('0') + 1;
            str.replace(first, str.size(), str.size() - first, ' ');
        }
    }

    auto str() {
        if (fixed) {
            auto str = ss.str();
            remove_trailling_zeros(str);
            return str;
        } else {
            return ss.str();
        }
    }

    int precision;
    bool fixed;
    int width;

    std::stringstream ss;
}; // class ElementFormater

template <class Array>
int get_width([[maybe_unused]] const Array &A, int fixed) {
    using T = typename Array::value_type;
    if constexpr (meta::is_std_tuple_v<T>) {
        return -1;
    } else if constexpr (meta::is_complex_v<T>) {
        if (!fixed) {
            return 15;
        } else {
            return 12;
        }
    } else {
        if (!fixed) {
            return 12;
        } else {
            return 12;
        }
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

        if (abs_non_zero.empty()) {
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
          edgeitems(3), suppress(false) {}

    char separator;
    int precision;
    // Total number of array elements which trigger summarization rather than full repr
    std::size_t threshold;
    // Number of characters per line for the purpose of inserting line breaks
    int linewidth;
    // Number of array items in summary at beginning and end of each dimension
    std::size_t edgeitems;
    // If true, always print floating point numbers using fixed point notation
    bool suppress;
};

template <class Stream, class Array, meta::enable_if_iterable<Array> = 0>
void print(Stream &stream, const Array &A, const PrintOptions &prtopts) {
    if (A.empty()) {
        stream << "[]\n";
        return;
    }

    stream << "[";
    int pos = 1;

    if (A.size() <= prtopts.threshold) {
        const auto fixed =
            prtopts.suppress || !detail::use_scientific_notation(A);
        const auto width = detail::get_width(A, fixed);
        detail::ElementFormater fmter(prtopts.precision, fixed, width);

        for (std::size_t i = 0; i < A.size() - 1; ++i) {
            fmter.print_element(A[i]);
            const auto str = fmter.str();
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
        fmter.print_element(A[A.size() - 1]);
        stream << fmter.str();
    } else {
        const auto fixed = prtopts.suppress ||
                           !detail::use_scientific_notation(
                               detail::get_edgeitems(A, prtopts.edgeitems));
        const auto width = detail::get_width(A, fixed);
        detail::ElementFormater fmter(prtopts.precision, fixed, width);

        for (std::size_t i = 0; i < prtopts.edgeitems; ++i) {
            fmter.print_element(A[i]);
            const auto str = fmter.str();
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

        for (std::size_t i = A.size() - prtopts.edgeitems; i < A.size() - 1;
             ++i) {
            fmter.print_element(A[i]);
            const auto str = fmter.str();
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
        fmter.print_element(A[A.size() - 1]);
        stream << fmter.str();
    }

    stream << "]\n";
}

template <class Stream, class T>
void print(Stream &stream, const T &A) {
    if constexpr (meta::is_iterable_v<T>) {
        print(stream, A, PrintOptions{});
    } else {
        detail::ElementFormater fmter(PrintOptions{}.precision, false, 12);
        fmter.print_element(A);
        stream << fmter.str();
        stream << "\n";
    }
}

template <class T>
void print(const T &A) {
    print(std::cout, A);
}

} // namespace scicpp

#endif // SCICPP_CORE_PRINT