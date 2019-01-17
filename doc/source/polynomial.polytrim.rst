.. _polynomial_polytrim:

scicpp::polynomial::polytrim
====================================

Defined in header <scicpp/polynomials.hpp>

Remove trailing coefficients smaller than *tol* from a polynomial.

.. function:: template <typename T> \
              void polytrim(std::vector<T> &P, T tol = 0)