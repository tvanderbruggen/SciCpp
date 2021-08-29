// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_HISTOGRAM
#define SCICPP_CORE_HISTOGRAM

#include "scicpp/core/constants.hpp"
#include "scicpp/core/equal.hpp"
#include "scicpp/core/macros.hpp"
#include "scicpp/core/maths.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/stats.hpp"
#include "scicpp/core/units/quantity.hpp"

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

namespace scicpp::stats {

//---------------------------------------------------------------------------------
// histogram_bin_edges
//---------------------------------------------------------------------------------

enum BinEdgesMethod : int { SCOTT, SQRT, RICE, STURGES, FD, DOANE, AUTO };

namespace detail {

template <BinEdgesMethod method, typename Array>
auto scicpp_pure bin_width(const Array &x) {
    scicpp_require(!x.empty());

    using T = typename Array::value_type;
    using raw_t = typename units::representation_t<T>;

    if constexpr (method == SQRT) {
        return ptp(x) / sqrt(x.size());
    } else if constexpr (method == SCOTT) {
        return cbrt(raw_t{24} * sqrt(pi<raw_t>) / raw_t(x.size())) * std(x);
    } else if constexpr (method == RICE) {
        return raw_t{0.5} * ptp(x) / cbrt(x.size());
    } else if constexpr (method == STURGES) {
        return ptp(x) / (log2(x.size()) + 1.0);
    } else if constexpr (method == FD) {
        // Freedman-Diaconis histogram bin estimator.
        return raw_t{2} * iqr(x) / cbrt(x.size());
    } else if constexpr (method == DOANE) {
        if (x.size() <= 2) {
            return T{0};
        }

        const auto sg1 = sqrt(raw_t{6} * (x.size() - 2) /
                              raw_t((x.size() + 1) * (x.size() + 3)));
        const auto g1 = units::value(skew(x));

        if (std::isnan(g1)) {
            return T{0};
        }

        return ptp(x) / (raw_t{1} + log2(x.size()) +
                         log2(raw_t{1} + absolute(g1) / sg1));
    } else { // AUTO
        const auto fd_bw = bin_width<FD>(x);
        const auto sturges_bw = bin_width<STURGES>(x);

        if (units::fpclassify(fd_bw) == FP_ZERO) {
            return sturges_bw;
        }

        return units::fmin(fd_bw, sturges_bw);
    }
}

template <typename Array>
auto scicpp_pure outer_edges(const Array &x) noexcept {
    using T = typename Array::value_type;

    if (x.empty()) {
        return std::make_pair(T{0}, T{1});
    }

    auto first_edge = amin(x);
    auto last_edge = amax(x);

    if (almost_equal(first_edge, last_edge)) {
        first_edge -= T{0.5};
        last_edge += T{0.5};
    }

    return std::make_pair(first_edge, last_edge);
}

} // namespace detail

template <BinEdgesMethod method, typename Array>
auto histogram_bin_edges(const Array &x) {
    using T = typename Array::value_type;

    if (x.empty()) {
        return linspace(T{0}, T{1}, 2);
    }

    const auto [first_edge, last_edge] = detail::outer_edges(x);
    const auto width = detail::bin_width<method>(x);

    if (units::fpclassify(width) == FP_ZERO) {
        return linspace(first_edge, last_edge, 2);
    }

    const auto n_equal_bins =
        std::size_t(units::value(ceil((last_edge - first_edge) / width)));
    return linspace(first_edge, last_edge, n_equal_bins + 1);
}

template <typename Array>
auto histogram_bin_edges(const Array &x, std::size_t nbins = 10) {
    const auto [first_edge, last_edge] = detail::outer_edges(x);
    return linspace(first_edge, last_edge, nbins + 1);
}

//---------------------------------------------------------------------------------
// histogram
//---------------------------------------------------------------------------------

constexpr bool UniformBins = true;
constexpr bool NonUniformBins = false;

template <bool use_uniform_bins = false,
          class InputIt,
          typename T = typename std::iterator_traits<InputIt>::value_type>
auto histogram(InputIt first, InputIt last, const std::vector<T> &bins) {
    using raw_t = typename units::representation_t<T>;

    if (bins.size() <= 1) {
        return empty<signed_size_t>();
    }

    scicpp_require(std::is_sorted(bins.cbegin(), bins.cend()));

    auto hist = zeros<signed_size_t>(bins.size() - 1);

    if constexpr (use_uniform_bins) {
        // No search required if uniformly distributed bins,
        // we can directly compute the index.

        const auto step = bins[1] - bins[0];
        scicpp_require(step > T{0});

        for (; first != last; ++first) {
            const auto pos = units::value((*first - bins.front()) / step);

            if (pos >= raw_t(hist.size())) {
                if (almost_equal(*first, bins.back())) {
                    // Last bin edge is included
                    ++hist.back();
                }

                continue;
            }

            if (pos >= raw_t{0}) {
                ++hist[std::size_t(pos)];
            }
        }
    } else {
        // This works for both uniform and non-uniform bins.

        for (; first != last; ++first) {
            const auto it =
                std::upper_bound(bins.cbegin(), bins.cend(), *first);

            if (it == bins.cend()) { // No bin found
                if (almost_equal(*first, bins.back())) {
                    // Last bin edge is included
                    ++hist.back();
                }

                continue;
            }

            const auto pos = std::distance(bins.cbegin(), it) - 1;

            if (pos >= raw_t{0}) {
                ++hist[std::size_t(pos)];
            }
        }
    }

    // We only return the histogram for this overload,
    // the bins being an input argument.

    return hist;
}

template <bool use_uniform_bins = false,
          typename Array,
          typename T = typename Array::value_type>
auto histogram(const Array &x, const std::vector<T> &bins) {
    return histogram<use_uniform_bins>(x.cbegin(), x.cend(), bins);
}

template <BinEdgesMethod method, typename Array>
auto histogram(const Array &x) {
    const auto bins = histogram_bin_edges<method>(x);
    return std::make_pair(histogram<UniformBins>(x, bins), bins);
}

template <typename Array>
auto histogram(const Array &x, std::size_t nbins = 10) {
    const auto bins = histogram_bin_edges(x, nbins);
    return std::make_pair(histogram<UniformBins>(x, bins), bins);
}

} // namespace scicpp::stats

#endif // SCICPP_CORE_HISTOGRAM