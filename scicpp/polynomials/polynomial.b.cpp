// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "polynomial.hpp"

#include "scicpp/random.hpp"

NONIUS_BENCHMARK("polynomials::polyval (Dynamic, 1000 coefs, 10000 values)",
                 [](nonius::chronometer meter) {
                     const auto c = scicpp::random::rand<double>(1000);
                     const auto x = scicpp::random::rand<double>(10000);

                     meter.measure(
                         [&]() { return scicpp::polynomials::polyval(x, c); });
                 })

// NONIUS_BENCHMARK("polynomials::mulx (1000 coefs)",
//                  [](nonius::chronometer meter) {
//                      const auto c = scicpp::random::rand<double>(1000);
//                      auto P = scicpp::polynomials::Polynomial(c);

//                      meter.measure([&]() { return P.mulx(); });
//                  })

// NONIUS_BENCHMARK("polynomials::polyfromroots (100 roots)",
//                  [](nonius::chronometer meter) {
//                      const auto roots = scicpp::random::rand<double>(100);

//                      meter.measure([&]() {
//                          return scicpp::polynomials::polyfromroots(roots);
//                      });
//                  })

// NONIUS_BENCHMARK("polynomials::polycompanion (Dynamic, 100 coefs)",
//                  [](nonius::chronometer meter) {
//                      const auto c = scicpp::random::rand<double>(100);

//                      meter.measure([&]() {
//                          return scicpp::polynomials::polycompanion(c);
//                      });
//                  })

// NONIUS_BENCHMARK("polynomials::polycompanion (Fixed size, 100 coefs)",
//                  [](nonius::chronometer meter) {
//                      const auto c = scicpp::random::rand<double, 100>();

//                      meter.measure([&]() {
//                          return scicpp::polynomials::polycompanion(c);
//                      });
//                  })

// NONIUS_BENCHMARK("polynomials::polyroots (Dynamic, 100 coefs)",
//                  [](nonius::chronometer meter) {
//                      const auto c = scicpp::random::rand<double>(100);

//                      meter.measure(
//                          [&]() { return scicpp::polynomials::polyroots(c); });
//                  })

// NONIUS_BENCHMARK("polynomials::polypow (Dynamic, 100 coefs, DIRECT)",
//                  [](nonius::chronometer meter) {
//                      const auto c = scicpp::random::rand<double>(100);

//                      meter.measure([&]() {
//                          return scicpp::polynomials::polypow(c, 100);
//                      });
//                  })

// NONIUS_BENCHMARK("polynomials::polypow (Dynamic, 100 coefs, FFT)",
//                  [](nonius::chronometer meter) {
//                      const auto c = scicpp::random::rand<double>(100);

//                      meter.measure([&]() {
//                          return scicpp::polynomials::polypow<
//                              scicpp::polynomials::ConvMethod::FFT>(c, 100);
//                      });
//                  })

// NONIUS_BENCHMARK("polynomials::polyroots (Fixed size, 100 coefs)",
//                  [](nonius::chronometer meter) {
//                      const auto c = scicpp::random::rand<double, 100>();

//                      meter.measure(
//                          [&]() { return scicpp::polynomials::polyroots(c); });
//                  })

// NONIUS_BENCHMARK("polynomials::polyvander (Dynamic, 1000 points, deg=100)",
//                  [](nonius::chronometer meter) {
//                      const auto x = scicpp::random::rand<double>(1000);

//                      meter.measure([&]() {
//                          return scicpp::polynomials::polyvander(x, 100);
//                      });
//                  })

// NONIUS_BENCHMARK("polynomials::polyfit (Dynamic, 1000 points, deg=10)",
//                  [](nonius::chronometer meter) {
//                      const auto x = scicpp::random::rand<double>(1000);
//                      const auto y = scicpp::random::rand<double>(1000);

//                      meter.measure([&]() {
//                          return scicpp::polynomials::polyfit(x, y, 10);
//                      });
//                  })

// NONIUS_BENCHMARK("polynomials::polyfit (Fixed size, 1000 points, deg=10)",
//                  [](nonius::chronometer meter) {
//                      const auto x = scicpp::random::rand<double, 1000>();
//                      const auto y = scicpp::random::rand<double, 1000>();

//                      meter.measure([&]() {
//                          return scicpp::polynomials::polyfit<10>(x, y);
//                      });
//                  })

// NONIUS_BENCHMARK("polynomials::polyfit (Dynamic, 1E6 points, deg=1 linear fit)",
//                  [](nonius::chronometer meter) {
//                      const auto x = scicpp::random::rand<double>(1000000);
//                      const auto y = scicpp::random::rand<double>(1000000);

//                      meter.measure([&]() {
//                          return scicpp::polynomials::polyfit(x, y, 1);
//                      });
//                  })