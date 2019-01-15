.. _basics_utils_eigen_to_eigen_matrix:

scicpp::utils::eigen::to_eigen_matrix
====================================

Defined in header <scicpp/utils.hpp>

Convert a standard container to an Eigen::Matrix.

--------------------------------------

.. function:: template <typename T> \
              Eigen::Matrix<T, Eigen::Dynamic, 1> to_eigen_matrix(const std::vector<T> &v, int size = -1)

Convert a std::vector to an Eigen::Matrix of dynamic size.

If a size parameter is specified then the output vector is cropped.

--------------------------------------

.. function:: template <int size = -1, typename T, std::size_t N> \
              Eigen::Matrix<T, M, 1> to_eigen_matrix(const std::array<T, N> &a)

Convert a std::array to an Eigen::Matrix of fixed size.

The output size is M = N if the size template parameter is not specified, else M = size.
