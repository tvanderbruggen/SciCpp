#include <scicpp/core.hpp>
#include <scicpp/plots.hpp>
#include <scicpp/signal.hpp>

// https://matplotlib.org/stable/gallery/lines_bars_and_markers/cohere.html#sphx-glr-gallery-lines-bars-and-markers-cohere-py

namespace sci = scicpp;
namespace plt = sci::plots;

int main() {
    using namespace sci::operators;
    using namespace sci::units::literals;

    const auto N = 1E4;     // Number of samples
    const auto fs = 1E2_Hz; // Sampling frequency
    const auto f0 = 10_Hz;  // Signal frequency

    const auto t = sci::arange(0.0, N) / fs;

    // White noises
    const auto nse1 = sci::random::randn<double, 2813981405>(t.size());
    const auto nse2 = sci::random::randn<double, 4130028572>(t.size());

    // Angular frequency (1 turn = 2 pi radians)
    const auto omega0 = 1_turn * f0;
    const auto s1 = sci::sin(omega0 * t) + nse1;
    const auto s2 = sci::sin(omega0 * t) + nse2;

    // Configure the spectrum analyzer
    auto spec =
        sci::signal::Spectrum{}.fs(fs).window(sci::signal::windows::Hann, 256);

    auto plot3 = plt::cohere(spec, s1, s2);

    plot3.size(1000, 500);
    plot3.color("green");
    plot3.xlabel("FREQUENCY (Hz)");
    plot3.ylabel("COHERENCE");
    plot3.show();
}