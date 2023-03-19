#include <cmath>
#include <scicpp/core.hpp>
#include <scicpp/plots.hpp>

namespace sci = scicpp;
namespace plt = sci::plots;

int main() {
    using namespace sci::operators;

    constexpr std::size_t data_size = 100;
    constexpr std::size_t data_num = 4;
    std::array<std::array<double, data_size>, data_num> data{};

    for (std::size_t i = 0; i < data.size(); ++i) {
        data[i] = sci::random::normal<data_size>(
            10.0 * std::sin(double(i - 1) / data_num), double(i + 5));
    }

    auto plot = plt::boxplot(data).capwidths(0.05);
    plot.show();
}