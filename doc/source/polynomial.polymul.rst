.. _polynomial_polymul:

scicpp::polynomial::polymul
====================================

Defined in header <scicpp/polynomials/polynomial.hpp>

Multiply two polynomials.

--------------------------------------

.. function:: template <typename T, std::size_t N, std::size_t M> \
              constexpr std::array<T, N + M - 1> polymul(const std::array<T, N> &P1, const std::array<T, M> &P2)

Polynomial coefficients are defined as two std::array P1 and P2.

Return the resulting polynomial coefficients in a std::array.

--------------------------------------

.. function:: template <ConvMethod method, typename T> \
              std::vector<T> polymul(const std::vector<T> &P1, const std::vector<T> &P2)

Polynomial coefficients are defined as two std::vector P1 and P2.

Return the resulting polynomial coefficients in a std::vector of size :expr:`P1.size() + P2.size() - 1`.

The template argument method define the method to use for polynomial
multiplications either using direct convolution (:code:`scicpp::polynomial::ConvMethod::DIRECT`, by default)
or using Fast Fourier Transform (:code:`scicpp::polynomial::ConvMethod::FFT`).