.. _linalg_lstsq:

scicpp::linalg::lstsq
====================================

Defined in header <scicpp/linalg.hpp>

Compute least-squares solution to equation :math:`Ax = b`.

----------------

.. function:: template <typename T, int M, int N> \
              std::array<T, N> lstsq(const Eigen::Matrix<T, M, N> &A, const std::array<T, M> &b)

----------------

.. function:: template <typename T> \
              std::vector<T> lstsq(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &A, const std::vector<T> &b)

----------------

Example
-------------------------

::

    #include <array>
    #include <Eigen/Dense>
    #include <scicpp/linalg.hpp>
    #include <scicpp/print.hpp>

    int main()
    {
        Eigen::Matrix<double, 4, 2> A;
        A << 0., 1.,
             1., 1.,
             2., 1.,
             3., 1.;

        const std::array b{-1., 0.2, 0.9, 2.1};
        const auto x = scicpp::linalg::lstsq(A, b);
        scicpp::print(x);
    }

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/scipy/reference/generated/scipy.linalg.lstsq.html>`_