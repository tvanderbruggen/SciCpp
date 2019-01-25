.. _core_map:

scicpp::map
====================================

Defined in header <scicpp/core.hpp>

Apply a function to each elements of an array.

Unary operations
-------------------------

--------------------------------------

.. function:: template <class Array, class UnaryOp> \
              Array && map(UnaryOp op, Array &&a)

--------------------------------------

.. function:: template <class Array, class UnaryOp> \
              Array map(UnaryOp op, const Array &a)

Binary operations
-------------------------

--------------------------------------

.. function:: template <class Array, class BinaryOp> \
              Array && map(BinaryOp op, Array &&a1, const Array &a2)

--------------------------------------

.. function:: template <class Array, class BinaryOp> \
              Array && map(BinaryOp op, const Array &a1, Array &&a2)

--------------------------------------

.. function:: template <class Array, class BinaryOp> \
              Array && map(BinaryOp op, Array &&a1, Array &&a2)

--------------------------------------

.. function:: template <class Array, class BinaryOp> \
              Array map(BinaryOp op, const Array &a1, const Array &a2)

--------------------------------------

Example
-------------------------

::

    #include <array>
    #include <complex>
    #include <functional>
    #include <vector>
    #include <scicpp/core.hpp>

    int main()
    {
        using namespace std::literals; // For std::complex notation

        // Unary operations

        // Negate all elements of an array
        auto neg = scicpp::map(std::negate<>(), std::array{1., 2., 3.});

        // Conjuguate all elements of a vector of complex numbers
        auto cj = scicpp::map([](auto z) { return std::conj(z); }, std::vector{1. + 2.i, 2. - 3.i});

        // Binary operations

        // Sum two arrays
        auto sum = scicpp::map(std::plus<>(), std::array{1., 2., 3.}, std::array{4., 5., 6.});

        // Compute the product of sinus between two vectors
        auto p = scicpp::map([](auto x, auto y) { return std::sin(x) * std::sin(y); },
                             std::vector{1., 2., 3.},
                             std::vector{4., 5., 6.});
    }
