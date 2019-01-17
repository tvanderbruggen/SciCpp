.. _core_random_randn:

scicpp::random::randn
====================================

Defined in header <scicpp/core.hpp>

Return random samples from a normal distribution centered
in zero with standard deviation 1.0.

----------------

.. function:: template <typename T, std::size_t N> \
              std::array<T, N> randn()

Generate a std::array of normaly distributed random numbers.

----------------

.. function:: template <typename T> \
              T randn()

Generate a single random number normaly distributed.

----------------

.. function:: template <typename T> \
              std::vector<T> randn(std::size_t N)

Generate a std::vector of normaly distributed random numbers.

----------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.random.randn.html#numpy.random.randn>`_