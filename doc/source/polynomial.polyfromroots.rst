.. _polynomial_polyfromroots:

scicpp::polynomial::polyfromroots
====================================

Defined in header <scicpp/polynomials/polynomial.hpp>

Generate a polynomial with given roots.

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              Polynomial<T> polyfromroots(const std::array<T, N> &roots)

Build a :expr:`Polynomial` from the roots provided in a std::array.

--------------------------------------

.. function:: template <typename T> \
              Polynomial<T> polyfromroots(const std::vector<T> &roots)

Build a :expr:`Polynomial` from the roots provided in a std::vector.

--------------------------------------