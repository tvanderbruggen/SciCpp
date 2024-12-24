#ifndef SCICPP_SIGNAL_SIGNALTOOLS
#define SCICPP_SIGNAL_SIGNALTOOLS

#include "scicpp/core/constants.hpp"
#include "scicpp/core/macros.hpp"
#include "scicpp/core/maths.hpp"
#include "scicpp/core/meta.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/range.hpp"
#include "scicpp/core/utils.hpp"
#include "scicpp/linalg/solve.hpp"
#include "scicpp/polynomials/polynomial.hpp"
#include "scicpp/signal/arraytools.hpp"
#include "scicpp/signal/convolve.hpp"

#include <complex>
#include <tuple>
#include <vector>

namespace scicpp::signal {

enum PadType : int { EVEN, ODD, CONSTANT, NONE };
// enum METHOD : int { PAD, GUST };

namespace detail {

template <PadType padtype,
          typename Array,
          typename DiffTp = typename Array::difference_type>
auto validate_pad(const Array &x, DiffTp ntaps, DiffTp padlen = -1) {
    if constexpr (padtype == PadType::NONE) {
        return std::tuple{std::vector(x.cbegin(), x.cend()), 0};
    } else {
        const auto edge = (padlen == -1) ? 3 * ntaps : padlen;
        scicpp_require(x.size() > std::size_t(edge));

        if (edge > 0) {
            if constexpr (padtype == PadType::EVEN) {
                return std::tuple{even_ext(x, edge), edge};
            } else if constexpr (padtype == PadType::ODD) {
                return std::tuple{odd_ext(x, edge), edge};
            } else {
                return std::tuple{const_ext(x, edge), edge};
            }
        } else {
            return std::tuple{std::vector(x.cbegin(), x.cend()), edge};
        }
    }
}

// cf.polynomial::polycompanion
// template <typename T> // tentative
// auto companion(const std::vector<T> &a) {
//     auto result = detail::eye<T>(a.size() - 1, 0, -1);
//     auto sliced_a = detail::slice(a, 1, a.size());
//     result.at(0) = -sliced_a / a[0];
//     return result;
// }

// template <typename T>
// void filt(std::vector<T> &b,
//           std::vector<T> &a,
//           const std::vector<T> &x,
//           std::vector<T> &y,
//           std::vector<T> &Z) {
//     using namespace scicpp::operators;
//     size_t len_b = b.size();
//     size_t len_x = x.size();

//     // Normalize the filter coefficients
//     T a0 = a[0];
//     b = b / a0;
//     a = a / a0;

//     for (size_t k = 0; k < len_x; k++) {
//         if (len_b > 1) {
//             y[k] = Z[0] + b[0] * x[k]; // Calculate first delay (output)

//             // Fill in middle delays
//             for (size_t n = 0; n < len_b - 2; n++) {
//                 Z[n] = Z[n + 1] + x[k] * b[n + 1] - y[k] * a[n + 1];
//             }

//             // Calculate last delay
//             Z[len_b - 2] = x[k] * b[len_b - 1] - y[k] * a[len_b - 1];
//         } else {
//             y[k] = x[k] * b[0];
//         }
//     }
// }

// template <typename T>
// void zfill(const std::vector<T> &src, std::vector<T> &dst, size_t n) {
//     dst.assign(n, T(0));
//     std::copy_n(src.begin(), std::min(src.size(), n), dst.begin());
// }

// template <typename T>
// auto _raw_filter(std::vector<T> &b,
//                  std::vector<T> &a,
//                  const std::vector<T> &x,
//                  const std::vector<T> &zi) {
//     size_t nfilt = std::max(a.size(), b.size());

//     std::vector<T> azfilled(nfilt);
//     std::vector<T> bzfilled(nfilt);
//     std::vector<T> y(x.size(), 0.0);

//     zfill(a, azfilled, nfilt);
//     zfill(b, bzfilled, nfilt);

//     std::vector<T> zfzfilled;

//     if (!zi.empty()) {
//         zfzfilled = zi;
//     } else {
//         zfzfilled.assign(nfilt - 1, T(0));
//     }

//     filt(bzfilled, azfilled, x, y, zfzfilled);

//     return std::make_tuple(y, zfzfilled);
// }
// template <typename T>
// auto _linear_filter(const std::vector<T>& b, const std::vector<T>& a, const std::vector<T>& x,
//     std::vector<T> Vi, int axis = 0)
// {
//     size_t na, nb, zi_size;
//     int input_flag = 0;

//     if (a[0] == T{0}) throw std::invalid_argument("a[0] is 0.0");

//     na = a.size();
//     nb = b.size();

//     zi_size = std::max(na, nb) - 1;

//     std::vector<T> Vf(zi_size);

//     return _raw_filter(b, a, x, Vi);
// }
} // namespace detail
// template <typename T>
// auto lfilter_zi(std::vector<T> b, std::vector<T> a) {

//     using namespace scicpp::linalg;
//     using namespace scicpp::operators;
//     using namespace scicpp::polynomial;

//     while ((a.size() > 1) && (a[0] == 0.0)) {
//         a = axis_slice<T>(
//             a, 1, a.size()); // we know BA only contains vector<double>'s
//     }
//     if (a.size() < 1)
//         throw std::invalid_argument(
//             "There must be at least one nonzero `a` coefficient.");
//     if (a[0] != 1.0) {
//         b = b / a[0];
//         a = a / a[0];
//     }
//     std::size_t n = std::max(a.size(), b.size());

//     if (a.size() < n)
//         a = detail::concatenate<T>(a, zeros<double>(n - a.size()));
//     else if (b.size() < n)
//         b = detail::concatenate<T>(b, zeros<double>(n - b.size()));

//     auto comp_a = detail::companion(a);

//     // transpose comp_a in place
//     for (int i = 0; i < comp_a.size(); i++)
//         for (int j = i + 1; j < comp_a.size(); j++)
//             std::swap(comp_a[i][j], comp_a[j][i]);

//     auto eye_n = detail::eye<T>(n - 1);
//     std::vector<std::vector<T>> IminusA;

//     for (int i = 0; i < comp_a.size(); i++)
//         IminusA.push_back(eye_n[i] - comp_a[i]);

//     Eigen::MatrixXd emi(IminusA.size(), IminusA.size());
//     for (int i = 0; i < IminusA.size(); i++)
//         for (int j = 0; j < IminusA.size(); j++)
//             emi(i, j) = IminusA[i][j];

//     auto B = detail::slice<T>(b, 1, b.size()) -
//              (detail::slice(a, 1, a.size()) * b[0]);

//     // Solve zi = A*zi + B
//     auto zi = lstsq(emi, B);

//     return zi;
// }
// template <typename T>
// auto lfilter(std::vector<T> &b,
//              std::vector<T> &a,
//              std::vector<T> &x,
//              std::vector<T> zi,
//              int axis = 0) {
//     // check a == 1
//     // using namespace scicpp::operators;
//     // if (a.size() == 1) {
//     //     b = b / a[0];
//     //     auto out_full = map([=](auto k) { return convolve(b, k); }, x);
//     // } else {
//     // }
//     return detail::_raw_filter(b, a, x, zi);
// }
// template <typename T, PADTYPE ptype = PADTYPE::ODD, METHOD method = METHOD::PAD>
// auto filtfilt(std::vector<T> &b,
//               std::vector<T> &a,
//               std::vector<T> &x,
//               int axis = 0,
//               int padlen = -1,
//               std::optional<int> irlen = std::nullopt) {
//     using namespace scicpp::operators;
//     if (method == METHOD::GUST)
//         ;
//     auto [ext, edge] = detail::_validate_pad<T, ptype>(
//         x, 0, std::max(a.size(), b.size()), padlen);

//     const auto zi = lfilter_zi(b, a);

//     T x0 = ext[0];

//     // Forward filter.
//     auto [y, zf] = lfilter(b, a, ext, zi * x0);

//     // Backward filter.
//     // Create y0 so zi*y0 broadcasts appropriately.
//     T y0 = y[y.size() - 1];
//     auto y_rev = utils::set_array(y);

//     std::reverse_copy(y.begin(), y.end(), y_rev.begin());

//     auto [y_new, zf_new] = lfilter(b, a, y_rev, zi * y0);

//     // Reverse y.
//     std::reverse(y_new.begin(), y_new.end());

//     if (edge > 0)
//         // Slice the actual signal from the extended signal.
//         y_new = axis_slice(y_new, edge, -edge);

//     return y_new;
// }

} // namespace scicpp::signal

#endif
