#ifndef AI_TOOLBOX_TYPES_HEADER_FILE
#define AI_TOOLBOX_TYPES_HEADER_FILE

#include <vector>
#include <unordered_map>
#include <random>

#include <boost/multi_array.hpp>

#include <Eigen/Core>
#include <Eigen/SparseCore>

namespace AIToolbox {
    // This should have decent properties.
    using RandomEngine = std::mt19937;

    using Table3D = boost::multi_array<double, 3>;
    using Table2D = boost::multi_array<double, 2>;

    using Matrix2D = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor | Eigen::AutoAlign>;
    using SparseMatrix2D = Eigen::SparseMatrix<double, Eigen::RowMajor>;
    using SparseMatrix2DLong = Eigen::SparseMatrix<long, Eigen::RowMajor>;

    using Vector   = Eigen::Matrix<double, Eigen::Dynamic, 1>;

    using Matrix3D = std::vector<Matrix2D>;
    using SparseMatrix3D = std::vector<SparseMatrix2D>;
    using SparseMatrix3DLong = std::vector<SparseMatrix2DLong>;

    using Matrix4D = boost::multi_array<Matrix2D, 2>;
    using SparseMatrix4D = boost::multi_array<SparseMatrix2D, 2>;

    // This is used to store a probability vector (sums to one, every element >= 0, <= 1)
    using ProbabilityVector = Vector;

    /**
     * @brief This struct is used to copy constness from one type to another.
     */
    template <typename CopiedType, typename ConstReference>
    struct copy_const {
        using type = typename std::conditional<std::is_const<ConstReference>::value,
                                      typename std::add_const<CopiedType>::type,
                                      typename std::remove_const<CopiedType>::type>::type;
    };
    template <typename CopiedType, typename ConstReference>
    using copy_const_t = typename copy_const<CopiedType, ConstReference>::type;

    /**
     * @brief This struct is used to both remove references and all cv qualifiers.
     */
    template <typename T>
    struct remove_cv_ref { using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type; };
    template <typename T>
    using remove_cv_ref_t = typename remove_cv_ref<T>::type;

    /**
     * @brief This is used to tag functions that avoid runtime checks.
     */
    inline struct NoCheck {} NO_CHECK;
}

#endif
