// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_UTILS
#define SCICPP_CORE_UTILS

#include <array>
#include <iterator>
#include <utility>
#include <vector>

namespace scicpp::utils {

//---------------------------------------------------------------------------------
// set_array: Set an array of same size than a given array
//---------------------------------------------------------------------------------

template <typename OutputType, typename T, std::size_t N>
auto set_array(const std::array<T, N> & /* unused */) {
    return std::array<OutputType, N>{};
}

template <typename OutputType, typename T>
auto set_array(std::vector<T> v) {
    return std::vector<OutputType>(v.size());
}

template <class Array>
auto set_array(const Array &a) {
    return set_array<typename Array::value_type>(a);
}

//---------------------------------------------------------------------------------
// move_sub_vector
//---------------------------------------------------------------------------------

template <typename T>
auto move_subvector(std::vector<T> &&v, std::size_t len) {
    return std::vector<T>(
        std::make_move_iterator(v.begin()),
        std::make_move_iterator(v.begin() + int(std::min(len, v.size()))));
}

//---------------------------------------------------------------------------------
// Compile-time prime factor decomposition
//---------------------------------------------------------------------------------

template <intmax_t n>
struct prime_factors {
    using prime_t = intmax_t;
    using pow_t = intmax_t;

    // https://stackoverflow.com/questions/11924249/finding-prime-factors
    constexpr prime_factors() {
        intmax_t z = 2;
        intmax_t num = n;

        while (z * z <= num) {
            if (num % z == 0) {
                append(z);
                num /= z;
            } else {
                z++;
            }
        }

        if (num > 1) {
            append(num);
        }
    }

    auto values() const { return factors; }

  private:
    // https://hbfs.wordpress.com/2016/03/22/log2-with-c-metaprogramming/
    static constexpr intmax_t ct_log2(intmax_t num) {
        return (num < 2) ? 1 : 1 + ct_log2(num / 2);
    }

    constexpr void append(prime_t prime) {
        std::size_t i = 0;

        for (; i < factors.size(); ++i) {
            if (factors[i].first == prime) {
                ++factors[i].second;
                return;
            }

            if (factors[i].first == 0) {
                break;
            }
        }

        // prime not in array
        factors[i] = {prime, 1};
    }

    // There is at most log2(n) prime factors
    std::array<std::pair<prime_t, intmax_t>, std::size_t(ct_log2(n))> factors{};

};

} // namespace scicpp::utils

#endif // SCICPP_CORE_UTILS