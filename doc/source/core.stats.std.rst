.. _core_stats_std:

scicpp::stats::std, nanstd, tstd
====================================

Defined in header <scicpp/core.hpp>

Compute the standard deviation. ddof is the `delta degrees of freedom <https://en.wikipedia.org/wiki/Degrees_of_freedom_(statistics)>`_

----------------

.. function:: template <int ddof = 0, class Array, class Predicate>\
              auto std(const Array &f, Predicate filter)

Compute the standard deviation of an array using only the values satisfying the filter predicate.


----------------

.. function:: template <int ddof = 0, class Array>\
              auto std(const Array &f)

Compute the standard deviation of an array.

----------------

.. function:: template <int ddof = 0, class Array>\
              auto nanstd(const Array &f)

Compute the standard deviation of an array, ignoring NaNs.

----------------

.. function:: template <int ddof = 1, class Array>\
              auto tstd(const Array &f, const std::array<T, 2> &limits, const std::array<bool, 2> &inclusive)

Compute the standard deviation of an array, ignoring values outside the given `limits`.
Use the optional parameter `inclusive` to specify whether the limits are included (by default they are included).
Note that by default here `ddof = 1`.

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.std.html#numpy.std>`_