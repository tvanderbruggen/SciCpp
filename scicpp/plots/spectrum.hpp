// SPDX-License-Identifier: MIT
// Copyright (c) 2022 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_PLOTS_CSD
#define SCICPP_PLOTS_CSD

#include "scicpp/signal/spectral.hpp"

#include <sciplot/sciplot.hpp>
#include <string>
#include <utility>

namespace scicpp::plots {

namespace detail {

template <typename FreqArray, typename PowerArray>
struct csdplot : sciplot::Plot2D {
  public:
    csdplot(FreqArray &&freqs, PowerArray &&power)
        : m_freqs(std::move(freqs)), m_power(std::move(power)) {
        redraw();
    }

    auto color(const std::string &color) {
        m_color = color;
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
    std::string m_color = "blue";
    FreqArray m_freqs;
    PowerArray m_power;

    void redraw() {
        clear();
        drawCurve(m_freqs, m_power).lineColor(m_color).labelNone();
        border().right().top();
    }

}; // class csdplot

} // namespace detail

template <signal::SpectrumScaling scaling = signal::DENSITY,
          typename Array1,
          typename Array2,
          typename T = double>
auto csd(signal::Spectrum<T> spec, const Array1 &x, const Array2 &y) {
    using namespace operators;
    auto [f, Pxy] = spec.template csd<scaling>(x, y);
    return detail::csdplot(std::move(f), T{10} * log10(norm(std::move(Pxy))));
}

template <signal::SpectrumScaling scaling = signal::DENSITY,
          typename Array,
          typename T = double>
auto psd(signal::Spectrum<T> spec, const Array &x) {
    using namespace operators;
    auto [f, Pxx] = spec.template welch<scaling>(x);
    return detail::csdplot(std::move(f), T{10} * log10(norm(std::move(Pxx))));
}

template <typename Array1, typename Array2, typename T = double>
auto cohere(signal::Spectrum<T> spec, const Array1 &x, const Array2 &y) {
    using namespace operators;
    auto [f, Cxy] = spec.coherence(x, y);
    return detail::csdplot(std::move(f), std::move(Cxy));
}

} // namespace scicpp::plots

#endif // SCICPP_PLOTS_CSD