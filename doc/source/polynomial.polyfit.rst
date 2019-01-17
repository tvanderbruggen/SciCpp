.. _polynomial_polyfit:

scicpp::polynomial::polyfit
====================================

Defined in header <scicpp/polynomials.hpp>

Least squares polynomial fit.

--------------------------------------

.. function:: template <int deg, typename T, std::size_t N> \
              std::array<T, deg + 1> polyfit(const std::array<T, N> &x, const std::array<T, N> &y)

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> polyfit(const std::vector<T> &x, const std::vector<T> &y, int deg)

--------------------------------------