.. _polynomial_polyint:

scicpp::polynomial::polyint
====================================

Defined in header <scicpp/polynomials.hpp>

Integrate a polynomial.

--------------------------------------

.. function:: template <int m = 1, typename T, std::size_t N> \
              constexpr std::array<T, N + m> polyint(const std::array<T, N> &P)

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> polyint(const std::vector<T> &P, int m = 1)