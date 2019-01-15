.. _basics_random_rand:

scicpp::random::rand
====================================

Defined in header <scicpp/random.hpp>

Return uniformly distributed random samples.

----------------

.. function:: template <typename T, std::size_t N> \
              std::array<T, N> rand()

Generate a std::array of uniformly distributed random numbers in the range [0, 1).

----------------

.. function:: template <typename T> \
              T rand()

Generate a single random number uniformly distributed in the range [0, 1).

----------------

.. function:: template <typename T> \
              std::vector<T> rand(std::size_t N)

Generate a std::vector of uniformly distributed random numbers in the range [0, 1).

----------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.random.rand.html#numpy.random.rand>`_