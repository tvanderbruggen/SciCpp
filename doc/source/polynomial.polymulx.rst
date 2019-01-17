.. _polynomial_polymulx:

scicpp::polynomial::polymulx
====================================

Defined in header <scicpp/polynomials.hpp>

Multiply a polynomial by :math:`X`.

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              std::array<T, N + 1> polymulx(const std::array<T, N> &P)

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> polymulx(const std::vector<T> &P)

Return a std::vector of size :expr:`P.size() + 1`.

--------------------------------------

.. function:: template <typename T> \
              void polymulx_inplace(std::vector<T> &P)

Perform the transformation **in place**.
The size of P is incremented by 1 so this may reallocate memory.