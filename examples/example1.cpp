#include <cstdio>
#include <scicpp/print.hpp>
#include <scicpp/range.hpp>
#include <scicpp/stats/stats.hpp>

using namespace scicpp;

int main() {
    // Fixed size (std::array)
    auto a = linspace<100>(0., 10.);
    print(a);
    printf("Mean: %f\n", stats::mean(a));
    printf("Var: %f\n\n", stats::var(a));

    // Dynamic size (std::vector)
    auto v = linspace(0., 10., 100000000);
    print(v);
    printf("Mean: %f\n", stats::mean(v));
    printf("Var: %f\n\n", stats::var(v));

    return 0;
}
