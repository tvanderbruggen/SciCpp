.. _polynomial_polycompanion:

scicpp::polynomial::polycompanion
====================================

Defined in header <scicpp/polynomials/polynomial.hpp>

Return the `companion matrix <https://en.wikipedia.org/wiki/Companion_matrix>`_ of a polynomial.

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              Eigen::Matrix<T, N - 1, N - 1> \
              polycompanion(const std::array<T, N> &P)

    Polynomial coefficients stored in a std::array.

    Return a square fixed size Eigen::Matrix of size the polynomial degree.

--------------------------------------

.. function:: template <typename T> \
              Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> \
              polycompanion(const std::vector<T> &P)

    Polynomial coefficients stored in a std::vector.

    Return a square dynamic size Eigen::Matrix of size the polynomial degree.