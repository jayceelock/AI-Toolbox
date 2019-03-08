#ifndef AI_TOOLBOX_UTILS_CORE_HEADER_FILE
#define AI_TOOLBOX_UTILS_CORE_HEADER_FILE

#include <cmath>
#include <limits>

#include <AIToolbox/Types.hpp>

#include <boost/functional/hash.hpp>

namespace AIToolbox {
    constexpr auto equalToleranceSmall = 0.000000001;
    /// This is the max absolute difference for which two values can be considered equal.
    /// This is a relative term used in the checkEqualGeneral functions, where
    /// two values may be considered equal if they are within this percentage
    /// of each other.
    constexpr auto equalToleranceGeneral = 0.00000000001;

    /**
     * @brief Copies a 3d container into another 3d container.
     *
     * The containers needs to support data access through
     * operator[]. In addition, the dimensions of the
     * containers must match the ones specified.
     *
     * This is important, as this function DOES NOT perform
     * any size checks on the containers.
     *
     * @tparam T Type of the input container.
     * @tparam U Type of the output container.
     * @param in Input container.
     * @param out Output container.
     * @param d1 First dimension of the containers.
     * @param d2 Second dimension of the containers.
     * @param d3 Third dimension of the containers.
     */
    template <typename T, typename U>
    void copyDumb3D(const T & in, U & out, const size_t d1, const size_t d2, const size_t d3) {
        for ( size_t i = 0; i < d1; ++i )
            for ( size_t j = 0; j < d2; ++j )
                for ( size_t x = 0; x < d3; ++x )
                    out[i][j][x] = in[i][j][x];
    }

    /**
     * @brief This function checks if two doubles near [0,1] are reasonably equal.
     *
     * If the numbers are not near [0,1], the result is not guaranteed to be
     * what may be expected. The order of the parameters is not important.
     *
     * @param a The first number to compare.
     * @param b The second number to compare.
     *
     * @return True if the two numbers are close enough, false otherwise.
     */
    inline bool checkEqualSmall(const double a, const double b) {
        return ( std::fabs(a - b) <= equalToleranceSmall );
    }

    /**
     * @brief This function checks if two doubles near [0,1] are reasonably different.
     *
     * If the numbers are not near [0,1], the result is not guaranteed to be
     * what may be expected. The order of the parameters is not important.
     *
     * @param a The first number to compare.
     * @param b The second number to compare.
     *
     * @return True if the two numbers are far away enough, false otherwise.
     */
    inline bool checkDifferentSmall(const double a, const double b) {
        return !checkEqualSmall(a,b);
    }

    /**
     * @brief This function checks if two doubles are reasonably equal.
     *
     * The order of the parameters is not important.
     *
     * @param a The first number to compare.
     * @param b The second number to compare.
     *
     * @return True if the two numbers are close enough, false otherwise.
     */
    inline bool checkEqualGeneral(const double a, const double b) {
        if ( checkEqualSmall(a,b) ) return true;
        return ( std::fabs(a - b) <= std::min(std::fabs(a), std::fabs(b)) * equalToleranceGeneral );
    }

    /**
     * @brief This function checks if two doubles are reasonably different.
     *
     * The order of the parameters is not important.
     *
     * @param a The first number to compare.
     * @param b The second number to compare.
     *
     * @return True if the two numbers are far away enough, false otherwise.
     */
    inline bool checkDifferentGeneral(const double a, const double b) {
        return !checkEqualGeneral(a,b);
    }

    /**
     * @brief This function checks whether a given vector only contains the stated value.
     *
     * This function compares using checkEqualSmall(double, double);
     *
     * @return True if all elements are compared equal to the input.
     */
    template <typename V>
    bool checkEqualSmall(const V & v, const double d) {
        for (decltype(v.size()) i = 0; i < v.size(); ++i)
            if (!checkEqualSmall(v[i], d)) return false;

        return true;
    }

    /**
     * @brief This function checks whether a given vector does not contain only the stated value.
     *
     * @return True if not all elements are equal to the input.
     */
    template <typename V>
    bool checkDifferentSmall(const V & v, const double d) {
        return !checkEqualSmall(v, d);
    }

    /**
     * @brief This function checks whether a given vector only contains the stated value.
     *
     * This function compares using checkEqualGeneral(double, double);
     *
     * @return True if all elements are compared equal to the input.
     */
    template <typename V>
    bool checkEqualGeneral(const V & v, const double d) {
        for (decltype(v.size()) i = 0; i < v.size(); ++i)
            if (!checkEqualGeneral(v[i], d)) return false;

        return true;
    }

    /**
     * @brief This function checks whether a given vector does not contain only the stated value.
     *
     * @return True if not all elements are equal to the input.
     */
    template <typename V>
    bool checkDifferentGeneral(const V & v, const double d) {
        return !checkEqualGeneral(v, d);
    }

    /**
     * @brief This function compares two general vectors of equal size lexicographically.
     *
     * Note that veccmp reports equality only if the elements are all exactly
     * the same. You should not use this function to compare floating point
     * numbers unless you know what you are doing.
     *
     * Note: This function assumes that the inputs are equally sized.
     *
     * @param lhs The left hand size of the comparison.
     * @param rhs The right hand size of the comparison.
     *
     * @return 1 if the lhs is greater than the rhs, 0 if they are equal, -1 otherwise.
     */
    template <typename V>
    int veccmp(const V & lhs, const V & rhs) {
        assert(lhs.size() == rhs.size());
        for (decltype(lhs.size()) i = 0; i < lhs.size(); ++i) {
            if (lhs[i] > rhs[i]) return 1;
            if (lhs[i] < rhs[i]) return -1;
        }
        return 0;
    }

    /**
     * @brief This function compares two general vectors of equal size lexicographically.
     *
     * Note that veccmpSmall considers two elements equal using the
     * checkEqualSmall function.
     *
     * Note: This function assumes that the inputs are equally sized.
     *
     * @param lhs The left hand size of the comparison.
     * @param rhs The right hand size of the comparison.
     *
     * @return 1 if the lhs is greater than the rhs, 0 if they are equal, -1 otherwise.
     */
    template <typename V>
    int veccmpSmall(const V & lhs, const V & rhs) {
        assert(lhs.size() == rhs.size());
        for (decltype(lhs.size()) i = 0; i < lhs.size(); ++i) {
            if (checkEqualSmall(lhs[i], rhs[i])) continue;
            return lhs[i] > rhs[i] ? 1 : -1;
        }
        return 0;
    }

    /**
     * @brief This function compares two general vectors of equal size lexicographically.
     *
     * This function considers two elements equal using the checkEqualGeneral
     * function.
     *
     * Note: This function assumes that the inputs are equally sized.
     *
     * @param lhs The left hand size of the comparison.
     * @param rhs The right hand size of the comparison.
     *
     * @return 1 if the lhs is greater than the rhs, 0 if they are equal, -1 otherwise.
     */
    template <typename V>
    int veccmpGeneral(const V & lhs, const V & rhs) {
        assert(lhs.size() == rhs.size());
        for (decltype(lhs.size()) i = 0; i < lhs.size(); ++i) {
            if (checkEqualGeneral(lhs[i], rhs[i])) continue;
            return lhs[i] > rhs[i] ? 1 : -1;
        }
        return 0;
    }

    /**
     * @brief This function returns whether a sorted range contains a given element, via sequential scan.
     *
     * The idea behind this function is that for small sorted vectors it is
     * faster to do a sequential scan rather than employing the heavy handed
     * technique of binary search.
     *
     * @tparam V The type of the vector to be scanned.
     * @param v The vector to be scanned.
     * @param elem The element to be looked for in the vector.
     *
     * @return True if the vector contains that element, false otherwise.
     */
    template <typename V>
    bool sequential_sorted_contains(const V & v, decltype(v[0]) elem) {
        // Maybe this could be done with iterators?
        for (const auto & e : v) {
            if (e < elem) continue;
            if (e == elem) return true;
            return false;
        }
        return false;
    }

    /**
     * @brief This function returns whether a sorted range contains another sorted range, via sequential scan.
     *
     * Note: This function assumes that the contained vector is smaller or
     * equal in size of the vector to be searched.
     *
     * @tparam V The type of the vector to be scanned.
     * @param v The vector to be scanned.
     * @param elems The vector that must be contained.
     *
     * @return True if the vector contains all elements from the input, false otherwise.
     */
    template <typename V>
    bool sequential_sorted_contains(const V & v, const V & elems) {
        assert(elems.size() <= v.size());

        if (v.size() == elems.size())
            return veccmp(v, elems) == 0;

        decltype(v.size()) i = 0, j = 0;
        while (j < elems.size()) {
            while (i < v.size() && v[i] < elems[j]) ++i;
            if (i == v.size() || v[i] > elems[j]) return false;
            ++i, ++j;
        }
        return j == elems.size();
    }

    /**
     * @brief This function is equivalent to std::max_element, but takes a unary function.
     *
     * This function can be called when doing a comparison between elements is
     * expensive, as they must be converted to some particular value every
     * single time.
     *
     * Instead, here we take a unary function which we apply once to every
     * element in the range, and we pick the one with the value that compares
     * the max.
     *
     * If there are duplicates, the first max is returned.
     *
     * @param begin The begin of the range to compare.
     * @param end The end of the range to compare.
     * @param unary_converter The unary function to apply to each item in the range.
     *
     * @return The iterator pointing to the max element in the range.
     */
    template <typename It, typename F>
    auto max_element_unary(It begin, const It end, F unary_converter) {
        if (begin == end) return std::make_pair(end, 0.0);
        auto retval = begin;
        double max = std::invoke(unary_converter, *begin);

        while (++begin != end) {
            auto newV = std::invoke(unary_converter, *begin);
            if (newV > max) {
                retval = begin;
                max = newV;
            }
        }
        return std::make_pair(retval, max);
    }
}

namespace Eigen {
    /**
     * @brief This function enables hashing of Vectors with boost::hash.
     */
    inline size_t hash_value(const AIToolbox::Vector & v) {
        return boost::hash_range(v.data(), v.data() + v.size());
    }
}

#endif
