
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief `cnl::fixed_point` specializations of \ref std::common_type

#if !defined(CNL_FIXED_POINT_COMMON_TYPE_H)
#define CNL_FIXED_POINT_COMMON_TYPE_H 1

#include "type.h"

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // implementation-specific definitions

    namespace _impl {
        namespace fp {
            namespace ct {

                ////////////////////////////////////////////////////////////////////////////////
                // cnl::_impl::fp::ct::common_type_mixed

                template<class Lhs, class Rhs, class _Enable = void>
                struct common_type_mixed;

                // given a fixed-point and an integer type,
                // generates a fixed-point type that is as big as both of them (or as close as possible)
                template<class LhsRep, int LhsExponent, class RhsInteger>
                struct common_type_mixed<fixed_point
                        <LhsRep, LhsExponent>, RhsInteger, _impl::enable_if_t<numeric_limits<RhsInteger>::is_integer>> : std::common_type<
                        fixed_point<LhsRep, LhsExponent>, fixed_point<RhsInteger, 0>> {
                };

                // given a fixed-point and a floating-point type,
                // generates a floating-point type that is as big as both of them (or as close as possible)
                template<class LhsRep, int LhsExponent, class Float>
                struct common_type_mixed<
                        fixed_point<LhsRep, LhsExponent>, Float,
                        _impl::enable_if_t<std::is_floating_point<Float>::value>>
                    : std::common_type<_impl::fp::float_of_same_size<LhsRep>, Float> {
                };
            }
        }
    }
}

/// standard library
namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::common_type<> specializations related to cnl::fixed_point<>

    /// \brief unary specialization of \ref std::common_type for \ref cnl::fixed_point
    /// \tparam cnl::fixed_point<Rep, Exponent> input type
    template<class Rep, int Exponent>
    struct common_type<cnl::fixed_point<Rep, Exponent>> {
        /// the common type of the input
        using type = cnl::fixed_point<
                typename std::common_type<Rep>::type,
                Exponent>;
    };

    /// \brief binary specialization of \ref std::common_type for \ref cnl::fixed_point
    /// \tparam cnl::fixed_point<LhsRep, LhsExponent> first input type
    /// \tparam Rhs second input type
    template<class LhsRep, int LhsExponent, class Rhs>
    struct common_type<cnl::fixed_point<LhsRep, LhsExponent>, Rhs> {
        static_assert(!cnl::_impl::is_fixed_point<Rhs>::value, "fixed-point Rhs type");

        /// the common type of the two inputs
        using type = typename cnl::_impl::fp::ct::common_type_mixed<cnl::fixed_point<LhsRep, LhsExponent>, Rhs>::type;
    };

    /// \brief binary specialization of \ref std::common_type for \ref cnl::fixed_point
    /// \tparam Lhs first input type
    /// \tparam cnl::fixed_point<RhsRep, RhsExponent> second input type
    template<class Lhs, class RhsRep, int RhsExponent>
    struct common_type<Lhs, cnl::fixed_point<RhsRep, RhsExponent>> {
        static_assert(!cnl::_impl::is_fixed_point<Lhs>::value, "fixed-point Lhs type");

        /// the common type of the two inputs
        using type = typename cnl::_impl::fp::ct::common_type_mixed<cnl::fixed_point<RhsRep, RhsExponent>, Lhs>::type;
    };

    /// \brief binary specialization of \ref std::common_type for \ref cnl::fixed_point
    /// \tparam cnl::fixed_point<LhsRep, LhsExponent> first input type
    /// \tparam cnl::fixed_point<RhsRep, RhsExponent> second input type
    template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
    struct common_type<cnl::fixed_point<LhsRep, LhsExponent>, cnl::fixed_point<RhsRep, RhsExponent>> {

        /// the common type of the two inputs
        using type = cnl::fixed_point<cnl::_impl::common_type_t<LhsRep, RhsRep>, cnl::_impl::min(LhsExponent, RhsExponent)>;
    };
}

#endif  // CNL_FIXED_POINT_COMMON_TYPE_H
