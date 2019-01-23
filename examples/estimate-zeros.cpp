// Estimate the zeros of a noisy sinusoid using a polynomial fit

#include <algorithm>
#include <array>
#include <cmath>
#include <scicpp/core.hpp>
#include <scicpp/polynomials.hpp>

namespace sci = scicpp;
using namespace sci::operators;

int main() {
    const auto x = sci::linspace<100>(-0.1, 2.1) * M_PI;
    const auto y = sci::sin(x) + 0.1 * sci::random::rand<double, 100>();

    const auto P = sci::polynomials::polyfit<3>(x, y);
    const auto zeros = sci::polynomials::polyroots(P);

    printf("Zeros: Expect 0, pi, 2 pi\n");
    sci::print(zeros);
}