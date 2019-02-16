#include <scicpp/core.hpp>
#include <cstdio>

int main()
{
    const std::array a{-1., 1., 2., 3.};

    // Compute the mean of the array
    auto m = scicpp::stats::mean(a);

    // Compute the mean of the positive values of the array
    auto m_pos = scicpp::stats::mean(a, [](auto x) { return x >= 0; });

    printf("m = %f, m_pos = %f\n", m, m_pos);
}
