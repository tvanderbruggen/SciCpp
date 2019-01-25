.. _polynomial_Polynomial:

scicpp::polynomial::Polynomial
====================================

Defined in header <scicpp/polynomials.hpp>

--------------------------------------

.. class:: template<typename T>  Polynomial

The Polynomial class provides operators for:

:arithmetic: :expr:`+`, :expr:`-`, :expr:`*`, :expr:`+=`, :expr:`-=`, :expr:`*=`

:comparison: :expr:`==` (exact equality, see also :code:`is_approx`), :expr:`!=`

:evaluation: :expr:`()`.

Constructors
-------------------------

--------------------------------------

.. function:: Polynomial(std::size_t deg)

Initialize a Polynomial of a degree deg with zero coefficients.

--------------------------------------

.. function:: Polynomial(const std::vector<T> &coef)

Initialize a Polynomial from a std::vector of coefficients.

:parameter coef: Polynomial coefficients in order of increasing degree, 
             i.e., :expr:`{1., 2., 3.}` give :math:`1 + 2 X + 3 X^{2}`.

--------------------------------------

.. function:: template <std::size_t N> \
            Polynomial(const std::array<T, N> &coef)

Initialize a Polynomial from a std::array of coefficients.

--------------------------------------

.. function:: template <class Iterator> \
              Polynomial(Iterator first, Iterator last)

Initialize a Polynomial from an iterator.

--------------------------------------

.. function:: Polynomial(std::initializer_list<T> l)

Initialize a Polynomial from an initializer list.

Methods
-------------------------

--------------------------------------

.. function:: std::size_t degree() const

Return the polynomial degree.

--------------------------------------

.. function:: const std::vector<T> &data() const

Return the polynomial coefficients in a std::vector of size degree + 1.

--------------------------------------

.. function:: void mulx()

Multiply the polynomial by :math:`X`.

--------------------------------------

.. function:: void trim(T tol = 0)

Remove trailing coefficients smaller than *tol* from the polynomial.

--------------------------------------

.. function:: void reserve(std::size_t n_coefs)

Reserve the memory to store a number n_coefs of coefficients.

--------------------------------------

.. function:: template <int rel_tol = 16> \
              bool is_approx(const Polynomial &rhs) const

Check whether two polynomials are approximately equal.
That is whether they have the same degree and if the 
relative distance between each corresponding coefficient 
in units of epsillon is smaller than rel_tol.

Example
-------------------------

::

    #include <cstdio>
    #include <scicpp/polynomials.hpp>

    int main()
    {
        scicpp::polynomial::Polynomial P1{1., 2., 3.};
        scicpp::polynomial::Polynomial P2{9., 5., 1.};
        auto P = 2 * P1 * P2;

        // Print the resulting polynomial degree
        printf("deg(P) = %u\n", P.degree());

        // Evaluate the polynomial for x = 3.14
        printf("P(3.14) = %f\n", P(3.14));
    }