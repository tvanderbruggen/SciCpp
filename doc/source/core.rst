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

:code:`exp`, :code:`expm1`, :code:`exp2`, :code:`log`, :code:`log2`, :code:`log10`, :code:`log1p`.

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

:code:`absolute`, :code:`sqrt`, :code:`cbrt`, :code:`pow`.

Arithmetic operators
-------------

Provides arithmetic operators :code:`+`, :code:`-`, :code:`*`, :code:`/`, :code:`%` for `std::array` and `std::vector`.

The statement :code:`using namespace scicpp::operators` must be included in the scope where
operators are used.

Ones and zeros
----------------

:ref:`empty <core_empty>`
    Return an empty :code:`std::vector` of a given type.

:ref:`zeros <core_zeros>`
    Return a new array of given shape and type, filled with zeros.

:ref:`ones <core_ones>`
    Return a new array of given shape and type, filled with ones.

:ref:`full <core_full>`
    Return a new array of given shape and type, filled with fill_value.

Ranges
-------------

:ref:`linspace <core_linspace>`
    Return evenly spaced numbers over a specified interval.

:ref:`logspace <core_logspace>`
    Return numbers spaced evenly on a log scale.

:ref:`arange <core_arange>`
    Return evenly spaced values within a given interval.

Create arrays from existing data
----------------

:ref:`fromstring <core_fromstring>`
    Load a vector from a string.

:ref:`TxtLoader <core_TxtLoader>`
    Load data from a character separated values text file.

Save arrays to file
----------------

:ref:`TxtSaver <core_txtsaver>`
    Save an array to a text file.

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

Searching
----------------

:ref:`argmax, nanargmax <core_argmax>`
    Returns the indices of the maximum value of an array.

:ref:`argmin, nanargmin <core_argmin>`
    Returns the indices of the minimum value of an array.

:ref:`argwhere <core_argwhere>`
    Returns the indices of array elements matching a given condition.

:ref:`nonzero <core_nonzero>`
    Return the indices of the elements that are non-zero.

Comparisons and Logical
----------------

Operators for element-wise comparison between an array and a scalar :code:`==`, :code:`!=`, :code:`>`, :code:`>=`, :code:`<`, :code:`<=`.

For element-wise comparison between arrays, operators are not available because the C++ standard defines them for lexicographical comparison.
The comparison functions (same as Numpy) can be used instead: :code:`equal`, :code:`not_equal`, :code:`less`, :code:`less_equal`, :code:`greater` and :code:`greater_equal`.

Operators :code:`&&` and :code:`||` are defined for element-wise logical operations.

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

:ref:`stats::quantile, nanquantile <core_stats_quantile>`
    Compute the q-th quantile.

:ref:`stats::percentile, nanpercentile <core_stats_percentile>`
    Compute the q-th percentile.

:ref:`stats::iqr, naniqr <core_stats_iqr>`
    Compute the interquartile range.

:ref:`stats::median, nanmedian <core_stats_median>`
    Compute the median.

:ref:`stats::mean, nanmean, tmean <core_stats_mean>`
    Compute the arithmetic mean.

:ref:`stats::gmean, nangmean <core_stats_gmean>`
    Compute the geometric mean.

:ref:`stats::var, nanvar, tvar <core_stats_var>`
    Compute the variance.

:ref:`stats::std, nanstd, tstd <core_stats_std>`
    Compute the standard deviation.

:ref:`stats::sem, nansem, tsem <core_stats_sem>`
    Compute the standard error of measurement.

:ref:`stats::moment, nanmoment <core_stats_moment>`
    Calculate the nth moment about the mean for a sample.

:ref:`stats::kurtosis, nankurtosis <core_stats_kurtosis>`
    Compute the kurtosis (Fisher or Pearson) of a dataset.

:ref:`stats::skew, nanskew <core_stats_skew>`
    Compute the sample skewness of a data set.

:ref:`stats::covariance, nancovariance <core_stats_covariance>`
    Compute the covariance between two data sets.

:ref:`stats::cov, nancov <core_stats_cov>`
    Compute the covariance matrix between two data sets.

:ref:`stats::histogram_bin_edges <core_stats_histogram_bin_edges>`
    Compute the the edges of the bins for an histogram.

:ref:`stats::histogram <core_stats_histogram>`
    Compute the histogram of a dataset.

Interpolate
-------------

:ref:`interpolate::interp1d <core_interpolate_interp1d>`
    Interpolate a 1D array.

Units
-------------

:ref:`units::quantity <core_units_quantity>`
    An arithmetic-like type representing a quantity.

:ref:`Quantities <core_units_defined_quantities>`
    Defined quantities.

:ref:`Units <core_units_defined_units>`
    Defined units.

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

:ref:`array2string <core_array2string>`
    Format a array to a string.

