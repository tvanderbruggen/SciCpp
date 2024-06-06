// SPDX-License-Identifier: MIT
// Copyright (c) 2024 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_PLOTS_WINVIS
#define SCICPP_PLOTS_WINVIS

#include "scicpp/core/manips.hpp"
#include "scicpp/core/maths.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/signal/fft.hpp"
#include "scicpp/signal/windows.hpp"

#include <sciplot/sciplot.hpp>
#include <string>
#include <utility>

namespace scicpp::plots {

namespace detail {

template <typename Array>
auto window_spectrum_db(const Array &window) {
    using T = typename Array::value_type;
    using namespace operators;

    // Add zero padding on both sides of the window before computing the FFT
    const auto padding = zeros<T>(5 * window.size());
    auto spec = norm(signal::rfft(padding | window | padding));
    return 10.0 * log10(std::move(spec) / spec[0]);
}

// Mainlobe width at 3 dB
template <typename Array>
auto mainlobe_width(const Array &winfft) {
    using T = typename Array::value_type;
    using namespace operators;
    return std::size_t(argmin(winfft + T(3), [](auto x) { return x >= T(0); }));
}

} // namespace detail

template <typename Array>
void winvis(const Array &window) {
    using T = typename Array::value_type;

    const auto x = linspace(T(0), T(window.size()), window.size());
    sciplot::Plot2D plot_win;
    plot_win.drawCurve(x, window).lineWidth(2).labelNone();
    plot_win.grid().lineType(-1).lineWidth(2).show();
    plot_win.xlabel("Samples");
    plot_win.ylabel("Amplitude");

    const auto f = signal::rfftfreq<T>(11 * window.size());
    const auto win_fft = detail::window_spectrum_db(window);

    const auto idx = detail::mainlobe_width(win_fft);
    const auto width = f[idx];
    const auto width_str = std::string("Mainlobe width (3 dB): ") + std::to_string(width);
    // print(width);

    sciplot::Plot2D plot_fft;
    plot_fft.drawCurve(f, win_fft).lineWidth(2).labelNone();
    plot_fft.grid().lineType(-1).lineWidth(2).show();
    plot_fft.yrange(-200.0, 3.0);
    plot_fft.xlabel("Nyquist frequency");
    plot_fft.ylabel("Magnitude (dB)");

    sciplot::Figure fig = {{plot_win, plot_fft}};
    fig.title(width_str);
    sciplot::Canvas canvas = {{fig}};
    canvas.size(1100, 500);
    canvas.show();
}

void winvis(signal::windows::Window win, std::size_t N = 128) {
    winvis(signal::windows::get_window<double>(win, N));
}

} // namespace scicpp::plots

#endif // SCICPP_PLOTS_WINVIS