.. _core_random_normal:

scicpp::random::normal
====================================

Defined in header <scicpp/core.hpp>

Draw random samples from a normal (Gaussian) distribution.

----------------

.. function:: template <typename T, std::size_t N> \
              std::array<T, N> normal(T mu, T sigma)

Generate a std::array of samples from a normal distribution with mean :code:`mu` and standard deviation :code:`sigma`.

----------------

.. function:: template <typename T> \
              std::vector<T> normal(T mu, T sigma, std::size_t N)

Generate a std::vector of samples from a normal distribution with mean :code:`mu` and standard deviation :code:`sigma`.

----------------

See also
    ----------
    `Numpy documentation <https://numpy.org/doc/stable/reference/random/generated/numpy.random.normal.html>`_