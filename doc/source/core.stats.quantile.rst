.. _core_stats_quantile:

scicpp::stats::quantile, nanquantile
====================================

Defined in header <scicpp/core.hpp>

Compute the q-th quantile.

Interpolation method to use when the desired quantile lies between two data points i < j::

    enum class QuantileInterp : int {
        LOWER,    // i
        HIGHER,   // j
        NEAREST,  // i or j, whichever is nearest.
        MIDPOINT, // (i + j) / 2.
        LINEAR    // i + (j - i) * fraction, where fraction is the fractional part of the index surrounded by i and j.
    };

----------------

.. function:: template <QuantileInterp interpolation = QuantileInterp::LINEAR, class InputIt, class Predicate, typename T>\
              auto quantile(InputIt first, InputIt last, T q, Predicate filter)

Compute the :expr:`q`-th quantile of data defined by a pair of iterators :expr:`first`, :expr:`last` satisfying the predicate :expr:`filter`.

----------------

.. function:: template <QuantileInterp interpolation = QuantileInterp::LINEAR, class Array, class Predicate, typename T>\
              auto quantile(const Array &f, T q, Predicate filter)

Compute the :expr:`q`-th quantile of data in the array :expr:`f` satisfying the predicate :expr:`filter`.

----------------

.. function:: template <QuantileInterp interpolation = QuantileInterp::LINEAR, class Array, typename T>\
              auto quantile(Array &&f, T q)

Compute the :expr:`q`-th quantile of data in the array :expr:`f`.

----------------

.. function:: template <QuantileInterp interpolation = QuantileInterp::LINEAR, class Array, typename T>\
              auto nanquantile(const Array &f, T q)

Compute the :expr:`q`-th quantile of data in the array :expr:`f` excluding NaN's.

----------------

See also
    ----------
    `Scipy documentation <https://numpy.org/doc/stable/reference/generated/numpy.quantile.html>`_