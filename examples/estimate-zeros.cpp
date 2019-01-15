// Estimate the zeros of a noisy sinusoid using a polynomial fit

#include <algorithm>
#include <array>
#include <cmath>
#include <scicpp/polynomials/polynomial.hpp>
#include <scicpp/random.hpp>
#include <scicpp/range.hpp>

using namespace scicpp;

int main() {
    const auto x = linspace<100>(-0.1, 2.1 * M_PI);
    std::array<double, x.size()> y{};

    std::transform(x.begin(), x.end(), y.begin(), [&](auto x) {
        return std::sin(x) + 0.1 * random::rand<double>();
    });

    const auto P = polynomials::polyfit<3>(x, y);
    const auto zeros = polynomials::polyroots(P);

    printf("Zeros: Expect 0, pi, 2 pi\n");
    for (const auto &r : zeros) {
        printf("%f ", r.real());
    }
    printf("\n");

    return 0;
}