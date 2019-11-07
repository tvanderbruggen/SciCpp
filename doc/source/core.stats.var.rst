.. _core_stats_var:

scicpp::stats::var
====================================

Defined in header <scicpp/core.hpp>

Compute the variance. ddof is the `delta degrees of freedom <https://en.wikipedia.org/wiki/Degrees_of_freedom_(statistics)>`_

----------------

.. function:: template <int ddof = 0, class Array, class Predicate>\
              constexpr auto var(const Array &f, Predicate filter)

Compute the variance of an array using only the values satisfying the filter predicate.

----------------

.. function:: template <int ddof = 0, class Array>\
              constexpr auto var(const Array &f)

Compute the variance of an array.

----------------

.. function:: template <int ddof = 0, class Array>\
              auto nanvar(const Array &f)

Compute the variance of an array, ignoring NaNs.

Example
-------------------------

::

    #include <cstdio>
    #include <scicpp/core.hpp>

    int main()
    {
        const std::array a{-1., 1., 2., 3.};

        // Compute the variance of the array
        const auto v = scicpp::stats::var(a);

        // Compute the variance of the positive values of the array
        const auto v_pos = scicpp::stats::var(a, [](auto x) { return x >= 0; });

        printf("v = %f, v_pos = %f\n", v, v_pos);
    }

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.var.html#numpy.var>`_