#include <cstdio>
#include <scicpp/random.hpp>
#include <scicpp/signal/fft.hpp>
#include <scicpp/signal/windows.hpp>
#include <scicpp/stats/stats.hpp>

using namespace scicpp;

int main() {
    // Generate a Gaussian white-noise
    auto noise = random::randn<double>(10000);
    printf(
        "Noise: mean = %f, std = %f\n", stats::mean(noise), stats::std(noise));
    auto psd = signal::power_spectrum_density(
        noise, 1.0, signal::windows::Window::Flattop);
    // The expected PSD for a white noise is 2 * sigma ^ 2
    printf("PSD = %f\n", stats::mean(psd));
    return 0;
}
