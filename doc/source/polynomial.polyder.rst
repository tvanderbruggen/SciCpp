.. _polynomial_polyder:

scicpp::polynomial::polyder
====================================

Defined in header <scicpp/polynomials.hpp>

Differentiate a polynomial.

--------------------------------------

.. function:: template <int m = 1, typename T, std::size_t N> \
              std::array<T, max(N - m, 0)> polyder(const std::array<T, N> &P)

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> polyder(const std::vector<T> &P, int m = 1)