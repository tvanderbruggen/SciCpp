.. _polynomial_polyroots:

scicpp::polynomial::polyroots
====================================

Defined in header <scicpp/polynomials.hpp>

Compute the roots of a polynomial.

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              std::array<std::complex<T>, N - 1> polyroots(const std::array<T, N> &P)

Compute the roots of a polynomial whose coefficients are stored in a std::array.

--------------------------------------

.. function:: template <typename T> \
              std::vector<std::complex<T>> polyroots(const std::vector<T> &P)

Compute the roots of a polynomial whose coefficients are stored in a std::vector.