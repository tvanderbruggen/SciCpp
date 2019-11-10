.. _core_stats_mean:

scicpp::stats::mean, nanmean, tmean
====================================

Defined in header <scicpp/core.hpp>

Compute the arithmetic mean.

----------------

.. function:: template <class Array, class Predicate>\
              constexpr auto mean(const Array &f, Predicate filter)

Compute the arithmetic mean of an array using only the values satisfying the filter predicate.

----------------

.. function:: template <class Array>\
              constexpr auto mean(const Array &f)

Compute the arithmetic mean of an array.

----------------

.. function:: template <class Array>\
              auto nanmean(const Array &f)

Compute the arithmetic mean of an array, ignoring NaNs.

----------------

.. function:: template <class Array>\
              constexpr auto tmean(const Array &f, const std::array<T, 2> &limits, const std::array<bool, 2> &inclusive)

Compute the arithmetic mean of an array, ignoring values outside the given `limits`.
Use the optional parameter `inclusive` to specify whether the limits are included (by default they are included).


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
    | `Scipy documentation for mean <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.mean.html#numpy.mean>`_
    | `Scipy documentation for nanmean <https://docs.scipy.org/doc/numpy/reference/generated/numpy.nanmean.html#numpy.nanmean>`_  
    | `Scipy documentation for tmean <https://docs.scipy.org/doc/scipy/reference/generated/scipy.stats.tmean.html#scipy.stats.tmean>`_