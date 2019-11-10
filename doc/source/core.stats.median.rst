.. _core_stats_median:

scicpp::stats::median
====================================

Defined in header <scicpp/core.hpp>

Compute the median.

----------------

.. function:: template <class Array, class Predicate>\
              auto median(const Array &f, Predicate filter)

Compute the median of an array using only the values satisfying the filter predicate.

----------------

.. function:: template <class Array>\
              auto median(Array &&f)

Compute the median of an array.

----------------

.. function:: template <class Array>\
              auto nanmedian(Array &&f)

Compute the median of an array, , ignoring NaNs.

Example
-------------------------

::

    #include <scicpp/core.hpp>
    #include <cstdio>

    int main()
    {
        const std::array a{-1., 1., 2., 3.};

        // Compute the mean of the array
        auto m = scicpp::stats::median(a);

        // Compute the mean of the positive values of the array
        auto m_pos = scicpp::stats::median(a, [](auto x) { return x >= 0; });

        printf("m = %f, m_pos = %f\n", m, m_pos);
    }


See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy/reference/generated/numpy.median.html#numpy.median>`_