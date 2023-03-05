#include <scicpp/core.hpp>
#include <scicpp/plots.hpp>

namespace sci = scicpp;
namespace plt = sci::plots;

int main() {
    using namespace sci::units::literals;
    using namespace sci::stats;

    const auto x = sci::random::normal<10000>(100., 15.);

    // Plot basic histogram
    auto plot0 = plt::hist(x);
    plot0.xlabel("Smarts");
    plot0.ylabel("Probability");
    plot0.fontSize(12);
    plot0.size(500, 500);
    plot0.show();
    // plot0.save("hist.pdf");

    // Plot logscale histogram with auto bin
    auto plot1 = plt::hist<BinEdgesMethod::AUTO>(x);
    plot1.color("grey");
    plot1.log(true);
    plot1.rwidth(0.75);
    plot1.xlabel("Smarts");
    plot1.ylabel("Probability");
    plot1.fontSize(12);
    plot1.size(500, 500);
    plot1.show();

    // Plot histogram of data with units
    const auto V = sci::random::normal<10000>(100_V, 15_V); // Voltage noise data
    auto plot2 = plt::hist<Density>(V);
    plot2.xlabel("Smarts");
    plot2.ylabel("Probability");
    plot2.fontSize(12);
    plot2.size(500, 500);
    plot2.show();
}