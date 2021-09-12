#include <scicpp/core.hpp>
#include <scicpp/plots.hpp>

int main() {
    namespace sci = scicpp;
    namespace plt = sci::plots;
    using namespace sci::operators;

    constexpr auto mu = 100.0;
    constexpr auto sigma = 15.0;
    const auto x = mu + sigma * sci::random::randn<double>(10000);

    auto plot = plt::hist(x);
    plot.color("grey");
    plot.xlabel("Smarts");
    plot.ylabel("Probability");
    plot.fontSize(12);
    plot.size(749, 749);
    plot.show();

    // plot.save("hist.pdf");
}