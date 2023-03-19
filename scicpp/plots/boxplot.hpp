// SPDX-License-Identifier: MIT
// Copyright (c) 2021-2022 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_PLOTS_BOXPLOT
#define SCICPP_PLOTS_BOXPLOT

#include "scicpp/core/functional.hpp"
#include "scicpp/core/stats.hpp"
#include "scicpp/core/utils.hpp"

#include <array>
#include <sciplot/sciplot.hpp>
#include <string>
#include <utility>
#include <vector>

namespace scicpp::plots {

namespace detail {

template <typename StatsVector>
struct boxplot : sciplot::Plot2D {
    static constexpr double default_boxwidth = 0.1;
    static constexpr double default_capwidths = 0.1;

  public:
    boxplot(StatsVector &&stats)
        : m_stats(std::move(stats)), m_widths(m_stats.size(), default_boxwidth),
          m_capwidths(m_stats.size(), default_capwidths) {
        redraw();
    }

    auto showcaps(bool showcaps) {
        m_showcaps = showcaps;
        redraw();
        return *this;
    }

    auto capwidths(double capwidths) {
        std::fill(m_widths.begin(), m_widths.end(), capwidths);
        redraw();
        return *this;
    }

    auto capwidths(std::vector<double> &&capwidths) {
        m_capwidths = std::move(capwidths);
        redraw();
        return *this;
    }

    auto capwidths(const std::vector<double> &capwidths) {
        m_capwidths = capwidths;
        redraw();
        return *this;
    }

    auto widths(double widths) {
        std::fill(m_widths.begin(), m_widths.end(), widths);
        redraw();
        return *this;
    }

    auto widths(std::vector<double> &&widths) {
        m_widths = std::move(widths);
        redraw();
        return *this;
    }

    auto widths(const std::vector<double> &widths) {
        m_widths = widths;
        redraw();
        return *this;
    }

    auto showbox(bool showbox) {
        m_showbox = showbox;
        redraw();
        return *this;
    }

    auto boxcolor(const std::string &boxcolor) {
        m_boxcolor = boxcolor;
        redraw();
        return *this;
    }

    auto canvas() {
        sciplot::Figure fig = {{*this}};
        sciplot::Canvas canvas = {{fig}};
        return canvas;
    }

    void show(std::size_t width = 750, std::size_t height = 600) {
        auto c = canvas();
        c.size(width, height);
        c.show();
    }

  private:
    std::string m_boxcolor = "blue"; // Could be an array
    std::string m_median_line_color = "orange";
    StatsVector m_stats;
    std::vector<double> m_widths;
    std::vector<double> m_capwidths;
    // double m_capwidths = 0.1;
    double m_whis = 1.5;
    bool m_showcaps = true;
    bool m_showbox = true;

    void redraw() {
        clear();

        for (std::size_t i = 0; i < m_stats.size(); ++i) {
            const auto [median, Q1, Q3] = m_stats[i];
            const auto x_line = std::array{double(i + 1) - m_widths[i],
                                           double(i + 1) + m_widths[i]};

            // Draw whiskers
            const auto iqr = Q3 - Q1;
            drawCurve(std::array{double(i + 1), double(i + 1)},
                      std::array{median - m_whis * iqr, median + m_whis * iqr})
                .lineColor("black");

            if (m_showcaps) {
                const auto x_line_caps =
                    std::array{double(i + 1) - m_capwidths[i],
                               double(i + 1) + m_capwidths[i]};
                drawCurve(
                    x_line_caps,
                    std::array{median - m_whis * iqr, median - m_whis * iqr})
                    .lineColor("black");
                drawCurve(
                    x_line_caps,
                    std::array{median + m_whis * iqr, median + m_whis * iqr})
                    .lineColor("black");
            }

            // Draw box
            if (m_showbox) {
                drawCurvesFilled(x_line, std::array{Q1, Q1}, std::array{Q3, Q3})
                    .fillSolid()
                    .fillColor(m_boxcolor)
                    .fillIntensity(0.5);

                // Draw border
                drawCurve(x_line, std::array{Q1, Q1}).lineColor(m_boxcolor);
                drawCurve(x_line, std::array{Q3, Q3}).lineColor(m_boxcolor);
                drawCurve(std::array{x_line[0], x_line[0]}, std::array{Q1, Q3}).lineColor(m_boxcolor);
                drawCurve(std::array{x_line[1], x_line[1]}, std::array{Q1, Q3}).lineColor(m_boxcolor);
            }

            // Draw median
            drawCurve(x_line, std::array{median, median})
                .lineColor(m_median_line_color);
        }

        border().right().top();
        legend().hide();
    }
}; // class boxplot

template <typename DataArray>
auto array_stats(const DataArray &f) {
    constexpr double rng0 = 25.0;
    constexpr double rng1 = 75.0;
    const auto Q1 = stats::percentile(f, rng0);
    const auto Q3 = stats::percentile(f, rng1);
    return std::array{stats::median(f), Q1, Q3};
}

template <typename Array>
auto stats(const Array &x) {
    return map([](auto v) { return array_stats(v); }, x);
}

} // namespace detail

// x: Array of array
template <typename Array>
auto boxplot(const Array &x) {
    return detail::boxplot(detail::stats(x));
}

} // namespace scicpp::plots

#endif // SCICPP_PLOTS_BOXPLOT