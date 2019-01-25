core
=============================

Core functions defined in header <scicpp/core.hpp>

Arithmetic operators
-------------

Provides arithmetic operators :code:`+`, :code:`-`, :code:`*`, :code:`/`, :code:`%` for `std::array` and `std::vector`.

The statement :code:`using namespace scicpp::operators` must be included in the scope where
operators are used.

Ranges
-------------

:ref:`linspace <core_linspace>`
    Return evenly spaced numbers over a specified interval.

:ref:`logspace <core_logspace>`
    Return numbers spaced evenly on a log scale.

:ref:`arange <core_arange>`
    Return evenly spaced values within a given interval.

Sums, products, differences
----------------

:ref:`sum <core_sum>`
    Sum of array elements.

:ref:`nansum <core_nansum>`
    Sum of array elements excluding NaNs.

:ref:`prod <core_prod>`
    Product of array elements.

:ref:`nanprod <core_nanprod>`
    Product of array elements excluding NaNs.

:ref:`cumsum <core_cumsum>`
    Cumulative sum of array elements.

:ref:`trapz <core_trapz>`
    Integrate using the trapezoidal rule.

:ref:`diff <core_diff>`
    The n-th discrete difference between consecutive elements of an array.

Comparisons
----------------

:ref:`almost_equal <core_almost_equal>`
    Compare floating points or arrays of floating points.

Random
-------------

:ref:`random::rand <core_random_rand>`
    Return uniformly distributed random samples.

:ref:`random::randn <core_random_randn>`
    Return normaly distributed random samples.

Statistics
-------------

:ref:`stats::amax <core_stats_amax>`
    Return the maximum value of an array.

:ref:`stats::amin <core_stats_amin>`
    Return the minimum value of an array.

:ref:`stats::ptp <core_stats_ptp>`
    Return the peak-to-peak span of an array.

:ref:`stats::average <core_stats_average>`
    Compute the weighted average.

:ref:`stats::mean <core_stats_mean>`
    Compute the arithmetic mean.

:ref:`stats::var <core_stats_var>`
    Compute the variance.

:ref:`stats::std <core_stats_std>`
    Compute the standard deviation.

Functional programming
---------------

:ref:`map <core_map>`
    Apply a function to each elements of an array.

:ref:`vectorize <core_vectorize>`
    Convert a function to accept array argument(s).

:ref:`filter <core_filter>`
    Filter a std::vector according to a predicate.

:ref:`filter_reduce <core_filter_reduce>`
    Reduce filtered elements of an array.

Printing
---------------

:ref:`print <core_print>`
    Print an array onto the screen.

:ref:`fprint <core_fprint>`
    Print an array to a file stream.

