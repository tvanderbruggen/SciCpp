.. _polynomial_polypow:

scicpp::polynomial::polypow
====================================

Defined in header <scicpp/polynomials/polynomial.hpp>

Raise a polynomial to a power.

--------------------------------------

.. function:: template <std::size_t pow, typename T, std::size_t N> \
              std::array<T, pow * (N - 1) + 1> polypow(const std::array<T, N> &P)

Polynomial coefficients are defined in a std::array P.

The power is the template argument pow.

--------------------------------------

.. function:: template <ConvMethod method, typename T> \
              std::vector<T> polypow(const std::vector<T> &P, std::size_t pow)

Polynomial coefficients are defined in a std::vector P
and the power is the function argument pow.

Return a std::vector of size :code:`pow * (P.size() - 1) + 1`.

The template argument method define the method to use for polynomial
multiplications either using direct convolution (:code:`scicpp::polynomial::ConvMethod::DIRECT`, by default)
or using Fast Fourier Transform (:code:`scicpp::polynomial::ConvMethod::FFT`).