.. _core_stats_var:

scicpp::stats::var
====================================

Defined in header <scicpp/core.hpp>

Compute the variance.

----------------

.. function:: template <class Array, class Predicate>
              constexpr auto var(const Array &f, Predicate filter)

Compute the variance of an array using only the values satisfying the filter predicate.

----------------

.. function:: template <class Array>
              constexpr auto var(const Array &f)

Compute the variance of an array.

Example
-------------------------

::

    #include <scicpp/core.hpp>

    int main()
    {
        const a = std::array{-1., 1., 2., 3.};

        // Compute the variance of the array
        auto m = scicpp::stats::var(a);

        // Compute the variance of the positive values of the array
        auto m_pos = scicpp::stats::var(a, [](auto x) { return x >= 0; });
    }

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.var.html#numpy.var>`_