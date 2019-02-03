.. _polynomial_polydiv:

scicpp::polynomial::polydiv
====================================

Defined in header <scicpp/polynomials.hpp>

Divide two polynomials.

Return a pair of two arrays :code:`[Q, R]` where :code:`Q` is the quotient and :code:`R`
is the remainder.

--------------------------------------

.. function:: template <typename T, std::size_t M, std::size_t N> \
              constexpr auto polymul(const std::array<T, M> &U, const std::array<T, N> &U)

--------------------------------------

.. function:: template <typename T> \
              auto polymul(const std::vector<T> &U, const std::vector<T> &V)

Example
-------------------------

::

    #include <scicpp/core.hpp>
    #include <scicpp/polynomials.hpp>

    int main()
    {
        const std::vector u{1., 2., 3.};
        const std::vector v{3., 2., 1.};
        const auto [q, r] = scicpp::polynomial::polydiv(u, v);

        print(q); // [ 3.0 ]
        print(r); // [ -8.0, -4.0 ]
    }