core
=============================

Core functions defined in header <scicpp/core.hpp>

:ref:`Quick overview comparing function calls for fixed and variable size arrays <core_fixed_vs_variable_size_array>`.

Mathematical functions
-------------

Provides vectorized versions of standard mathematical functions.

A vectorized version applies the function element-wise on the input array(s).

Function naming follows the `NumPy definitions <https://docs.scipy.org/doc/numpy-1.15.0/reference/routines.math.html>`_, 
which sometimes differs from the C++ definitions (ex. *arcsin* vs *asin*).

Trigonometric functions
^^^^^^^^^^^^^^^^

:code:`sin`, :code:`cos`, :code:`tan`, :code:`arcsin`, :code:`arccos`, :code:`arctan`, :code:`arctan2`, :code:`hypot`.

Hyperbolic functions
^^^^^^^^^^^^^^^^
:code:`sinh`, :code:`cosh`, :code:`tanh`, :code:`arcsinh`, :code:`arccosh`, :code:`arctanh`.

Exponents and logarithms
^^^^^^^^^^^^^^^^

:code:`exp`, :code:`expm1`, :code:`exp2`, :code:`log`, :code:`log2`, :code:`log1p`.

Rounding
^^^^^^^^^^^^^^^^
:code:`around`, :code:`floor`, :code:`ceil`, :code:`trunc`, :code:`rint`.

Complex numbers
^^^^^^^^^^^^^^^^

:code:`real`, :code:`imag`, :code:`angle`, :code:`conj`, :code:`norm`, :code:`polar`

NB: :code:`norm` and :code:`polar` are not provided by NumPy,
but are vectorized versions of the `std::complex <https://en.cppreference.com/w/cpp/numeric/complex>`_ functions.

Rational routines
^^^^^^^^^^^^^^^^

:code:`gcd`, :code:`lcm`.

Miscellaneous
^^^^^^^^^^^^^^^^

:code:`absolute`, :code:`sqrt`, :code:`cbrt`.

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

:ref:`nancumsum <core_nancumsum>`
    Cumulative sum of array elements excluding NaNs.

:ref:`cumprod <core_cumprod>`
    Cumulative products of array elements.

:ref:`nancumprod <core_nancumprod>`
    Cumulative products of array elements excluding NaNs.

:ref:`trapz <core_trapz>`
    Integrate using the trapezoidal rule.

:ref:`diff <core_diff>`
    The n-th discrete difference between consecutive elements of an array.

:ref:`inner or dot <core_inner>`
    Ordinary inner product of arrays (without complex conjugation).

:ref:`vdot <core_vdot>`
    Return the dot product of two vectors.

Comparisons
----------------

:ref:`almost_equal <core_almost_equal>`
    Compare floating points or arrays of floating points.

:ref:`array_equal <core_array_equal>`
    True if two arrays have the same size and elements (strict equality).

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

:ref:`stats::nanmean <core_stats_nanmean>`
    Compute the arithmetic mean, ignoring NaNs.

:ref:`stats::var <core_stats_var>`
    Compute the variance.

:ref:`stats::nanvar <core_stats_nanvar>`
    Compute the variance, ignoring NaNs.

:ref:`stats::std <core_stats_std>`
    Compute the standard deviation.

:ref:`stats::nanstd <core_stats_nanstd>`
    Compute the standard deviation, ignoring NaNs.

Units
-------------

:ref:`units::quantity <core_units_quantity>`
    An arithmetic-like type representing a quantity.

Masking
----------------

:ref:`mask <core_mask>`
    Return a vector with masked values.

:ref:`mask_array <core_mask_array>`
    Mask a vector in-place.

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

:ref:`reduce <core_reduce>`
    Reduce elements of an array.

:ref:`cumacc <core_cumacc>`
    Cumulative accumulation of array elements.

Printing
---------------

:ref:`print <core_print>`
    Print an array onto the screen.

:ref:`fprint <core_fprint>`
    Print an array to a file stream.

