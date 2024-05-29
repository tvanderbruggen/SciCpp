#include <scicpp/core.hpp>
#include <scicpp/plots.hpp>

namespace sci = scicpp;
namespace sig = sci::signal;
namespace plt = sci::plots;

using namespace sci::operators;

template <int n_extend, typename Array>
auto window_spectrum_db(const Array &win) {
    // Add zero padding on both sides of the window before computing the FFT
    const auto padding = sci::zeros<n_extend / 2, double>();
    auto spec = sci::norm(sig::rfft(padding | win | padding));
    return 10.0 * sci::log10(std::move(spec) / spec[0]);
}

int main() {
    constexpr auto N = 100;

    const auto x = sci::linspace<N>(-1.0, 1.0);
    const auto y_cosine = sig::windows::cosine<double, N>();
    const auto y_bohman = sig::windows::bohman<double, N>();
    const auto y_hann = sig::windows::hann<double, N>();
    const auto y_flattop = sig::windows::flattop<double, N>();

    auto plot = plt::plot(x, y_cosine, y_bohman, y_hann, y_flattop);
    plot.show();

    // ------------------------------------------------------
    // Window frequency response

    constexpr auto n_extend = 10 * N;
    const auto f = sig::rfftfreq<N + n_extend, double>();

    const auto fft_cosine = window_spectrum_db<n_extend>(y_cosine);
    const auto fft_bohman = window_spectrum_db<n_extend>(y_bohman);
    const auto fft_hann = window_spectrum_db<n_extend>(y_hann);
    const auto fft_flattop = window_spectrum_db<n_extend>(y_flattop);

    auto fft_plot = plt::plot(f, fft_cosine, fft_bohman, fft_hann, fft_flattop);
    fft_plot.show();
}