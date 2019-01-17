.. _polynomial_polyvander:

scicpp::polynomial::polyvander
====================================

Defined in header <scicpp/polynomials/polynomial.hpp>Defined in header <scicpp/polynomials.hpp>
Vandermonde matrix of given degree.

--------------------------------------

.. function:: template <int deg, typename T, std::size_t N> \
              Eigen::Matrix<T, N, deg + 1> \
              polyvander(const std::array<T, N> &x)

--------------------------------------

.. function:: template <typename T> \
              Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> \
              polyvander(const std::vector<T> &x, int deg)