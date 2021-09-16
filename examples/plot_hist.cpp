#include <scicpp/core.hpp>
#include <scicpp/plots.hpp>

namespace sci = scicpp;
namespace plt = sci::plots;

template <std::size_t N, typename T>
auto gaussian_noise(T mu, T sigma) {
    using namespace sci::operators;
    return mu + sigma * sci::random::randn<T, N>();
}

int main() {
    const auto x = gaussian_noise<10000>(100., 15.);

    // Plot basic histogram
    auto plot0 = plt::hist(x);
    plot0.xlabel("Smarts");
    plot0.ylabel("Probability");
    plot0.fontSize(12);
    plot0.size(500, 500);
    plot0.show();

    // Plot logscale histogram with auto bin
    auto plot1 = plt::hist<sci::stats::BinEdgesMethod::AUTO>(x);
    plot1.color("grey");
    plot1.log(true);
    plot1.rwidth(0.75);
    plot1.xlabel("Smarts");
    plot1.ylabel("Probability");
    plot1.fontSize(12);
    plot1.size(500, 500);
    plot1.show();

    // plot.save("hist.pdf");
}