// SPDX-License-Identifier: MIT
// Copyright (c) 2024 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_PLOTS_PLOT
#define SCICPP_PLOTS_PLOT

#include "scicpp/core/macros.hpp"
#include "scicpp/core/units/quantity.hpp"

#include <sciplot/sciplot.hpp>
#include <string>

namespace scicpp::plots {

namespace detail {

template <typename XArray, typename YArray>
struct plot : sciplot::Plot2D {
    using XTp = typename XArray::value_type;
    using YTp = typename XArray::value_type;
    using XRepTp = units::representation_t<XTp>;
    using YRepTp = units::representation_t<YTp>;

  public:
    plot(const XArray &x, const YArray &y)
        : m_x(sciplot::Vec(reinterpret_cast<const XRepTp *>(x.data()),
                           x.size())),
          m_y(sciplot::Vec(reinterpret_cast<const YRepTp *>(y.data()),
                           y.size())) {
        scicpp_require(m_x.size() == m_y.size());
        redraw();
    }

    auto color(const std::string &color) {
        m_color = color;
        redraw();
        return *this;
    }

    auto display_grid(bool display_grid) {
        m_display_grid = display_grid;
        redraw();
        return *this;
    }

    auto canvas() {
        sciplot::Figure fig = {{*this}};
        sciplot::Canvas canvas = {{fig}};
        return canvas;
    }

    auto label(const std::string &label) {
        m_label = label;
        redraw();
        return *this;
    }

    void show(std::size_t width = 750, std::size_t height = 600) {
        auto c = canvas();
        c.size(width, height);
        c.show();
    }

  private:
    std::string m_color = "blue";
    std::string m_label = "";
    bool m_display_grid = true;
    sciplot::Vec m_x;
    sciplot::Vec m_y;

    void redraw() {
        clear();

        if (m_label.empty()) {
            drawCurve(m_x, m_y).lineColor(m_color).labelNone();
        } else {
            drawCurve(m_x, m_y).lineColor(m_color).label(m_label);
        }

        if (m_display_grid) {
            grid().lineType(-1).lineWidth(2).show();
        } else {
            grid().hide();
        }

        border().right().top();
    }

}; // class plot

} // namespace detail

template <typename XArray, typename YArray>
auto plot(const XArray &x, const YArray &y) {
    return detail::plot(x, y);
}

} // namespace scicpp::plots

#endif // SCICPP_PLOTS_PLOT