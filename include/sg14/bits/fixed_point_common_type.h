
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief `sg14::fixed_point` specializations of \c std::common_type

#if !defined(SG14_FIXED_POINT_COMMON_TYPE_H)
#define SG14_FIXED_POINT_COMMON_TYPE_H 1

#include "fixed_point_type.h"

/// study group 14 of the C++ working group
namespace sg14 {

    ////////////////////////////////////////////////////////////////////////////////
    // implementation-specific definitions

    namespace _impl {
        namespace fp {
            namespace ct {

                ////////////////////////////////////////////////////////////////////////////////
                // sg14::_impl::fp::ct::common_type_mixed

                template<class Lhs, class Rhs, class _Enable = void>
                struct common_type_mixed;

                // given a fixed-point and an integer type,
                // generates a fixed-point type that is as big as both of them (or as close as possible)
                template<class LhsRep, int LhsExponent, class RhsInteger>
                struct common_type_mixed<fixed_point
                        <LhsRep, LhsExponent>, RhsInteger, _impl::enable_if_t<std::numeric_limits<RhsInteger>::is_integer>> : std::common_type<
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

namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::common_type<> specializations related to sg14::sg14::fixed_point<>

    // std::common_type<fixed_point<>>
    template<class Rep, int Exponent>
    struct common_type<sg14::fixed_point<Rep, Exponent>> {
        using type = sg14::fixed_point<
                typename std::common_type<Rep>::type,
                Exponent>;
    };

    // std::common_type<fixed_point<>, not-fixed-point>
    template<class LhsRep, int LhsExponent, class Rhs>
    struct common_type<sg14::fixed_point<LhsRep, LhsExponent>, Rhs> {
        static_assert(!sg14::_impl::is_fixed_point<Rhs>::value, "fixed-point Rhs type");
        using type = typename sg14::_impl::fp::ct::common_type_mixed<sg14::fixed_point<LhsRep, LhsExponent>, Rhs>::type;
    };

    // std::common_type<not-fixed-point, fixed_point<>>
    template<class Lhs, class RhsRep, int RhsExponent>
    struct common_type<Lhs, sg14::fixed_point<RhsRep, RhsExponent>> {
        static_assert(!sg14::_impl::is_fixed_point<Lhs>::value, "fixed-point Lhs type");
        using type = typename sg14::_impl::fp::ct::common_type_mixed<sg14::fixed_point<RhsRep, RhsExponent>, Lhs>::type;
    };

    // std::common_type<fixed_point<>, fixed_point<>>
    template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
    struct common_type<sg14::fixed_point<LhsRep, LhsExponent>, sg14::fixed_point<RhsRep, RhsExponent>> {
        using type = sg14::fixed_point<sg14::_impl::common_type_t<LhsRep, RhsRep>, sg14::_impl::min(LhsExponent, RhsExponent)>;
    };
}

#endif	// SG14_FIXED_POINT_COMMON_TYPE_H
