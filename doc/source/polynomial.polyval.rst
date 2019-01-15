.. _polynomial_polyval:

scicpp::polynomial::polyval
====================================

Defined in header <scicpp/polynomials/polynomial.hpp>

Evaluate a polynomial at points x.

Evaluate at a single point
-----------------------------

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              T polyval(T x, const std::array<T, N> &coeffs)

Evaluate a polynomial with coefficients defined in a std::array.

--------------------------------------

.. function:: template <typename T> \
              T polyval(T x, const std::vector<T> &coeffs)

Evaluate a polynomial with coefficients defined in a std::vector.

Evaluate over a range of points
--------------------------------

--------------------------------------

.. function:: template <class InputIt, class OutputIt, class Coeffs> \
              void polyval(InputIt xfirst, InputIt xlast, OutputIt res_first, const Coeffs &coeffs)

Evaluate a polynomial over an iterator range.

The memory pointed by res_first must be allocated
with appropriate size before calling the function.

:Parameters:
:xfirst: First point of the range.
:xlast: Last point of the range.
:res_first: First point where the evaluation results are stored.
:coeffs: An array containing the polynomial coefficients (std::array or std::vector)

--------------------------------------

.. function:: template <typename T, std::size_t M, class Coeffs> \
             std::array<T, M> polyval(const std::array<T, M> &x, const Coeffs &coeffs)

Evaluate a polynomial over a std::array.

:Parameters:
:x: The std::array of points to be evaluated.
:coeffs: An array containing the polynomial coefficients (std::array or std::vector)

--------------------------------------

.. function:: template <typename T, class Coeffs> \
             std::vector<T> polyval(const std::vector<T> &x, const Coeffs &coeffs)

Evaluate a polynomial over a std::vector.

:Parameters:
:x: The std::vector of points to be evaluated.
:coeffs: An array containing the polynomial coefficients (std::array or std::vector)
