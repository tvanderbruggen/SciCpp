.. _polynomial_polyadd:

scicpp::polynomial::polyadd
====================================

Defined in header <scicpp/polynomials/polynomial.hpp>

Add two polynomials.

--------------------------------------

.. function:: template <typename T, std::size_t M, std::size_t N> \
              constexpr std::array<T, (M > N ? M : N)> polyadd(const std::array<T, M> &P1, const std::array<T, N> &P2)

Polynomial coefficients are defined as two std::array P1 and P2.

Return the resulting polynomial coefficients in a std::array of size the largest size between M and N.

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> polyadd(const std::vector<T> &P1, const std::vector<T> &P2)

Polynomial coefficients are defined as two std::vector P1 and P2.

Return the resulting polynomial coefficients in a std::vector of size the
largest size between :expr:`P1.size()` and :expr:`P2.size()`.
