.. _core_stats_mean:

scicpp::stats::mean
====================================

Defined in header <scicpp/core.hpp>

Compute the arithmetic mean.

----------------

.. function:: template <class Array, class Predicate>
              constexpr auto mean(const Array &f, Predicate filter)

Compute the arithmetic mean of an array using only the values satisfying the filter predicate.

----------------

.. function:: template <class Array>
              constexpr auto mean(const Array &f)

Compute the arithmetic mean of an array.

Example
-------------------------

::

    #include <scicpp/core.hpp>
    #include <cstdio>

    int main()
    {
        const std::array a{-1., 1., 2., 3.};

        // Compute the mean of the array
        auto m = scicpp::stats::mean(a);

        // Compute the mean of the positive values of the array
        auto m_pos = scicpp::stats::mean(a, [](auto x) { return x >= 0; });

        printf("m = %f, m_pos = %f\n", m, m_pos);
    }


See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.mean.html#numpy.mean>`_