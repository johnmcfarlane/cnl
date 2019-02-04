
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief `cnl::fixed_point` operators

#if !defined(CNL_FIXED_POINT_OPERATORS_H)
#define CNL_FIXED_POINT_OPERATORS_H 1

#include "type.h"

#include <numeric>

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // heterogeneous operator overloads
    //
    // compare two objects of different fixed_point specializations

    namespace _impl {
        template<typename Operator, typename Rep, int Exponent, int Radix>
        struct unary_operator<Operator, fixed_point<Rep, Exponent, Radix>> {
            constexpr auto operator()(fixed_point<Rep, Exponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<decltype(Operator()(_impl::to_rep(rhs))), Exponent, Radix>>(Operator()(_impl::to_rep(rhs))))
            {
                return from_rep<fixed_point<decltype(Operator()(_impl::to_rep(rhs))), Exponent, Radix>>(Operator()(_impl::to_rep(rhs)));
            }
        };

        // comparison between operands with different rep
        template<typename Operator, typename LhsRep, typename RhsRep, int Exponent, int Radix>
        struct comparison_operator<Operator,
                fixed_point<LhsRep, Exponent, Radix>,
                fixed_point<RhsRep, Exponent, Radix>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, Exponent, Radix> const& lhs,
                    fixed_point<RhsRep, Exponent, Radix> const& rhs) const
            -> decltype(Operator{}(_impl::to_rep(lhs), _impl::to_rep(rhs)))
            {
                return Operator{}(_impl::to_rep(lhs), _impl::to_rep(rhs));
            }
        };

        // comparison between operands with different rep and exponent
        template<typename Operator, typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct comparison_operator<Operator,
                fixed_point<LhsRep, LhsExponent, Radix>,
                fixed_point<RhsRep, RhsExponent, Radix>,
                enable_if_t<(LhsExponent<RhsExponent)>> {
            static constexpr int shiftage = RhsExponent - LhsExponent;
            using lhs_type = fixed_point<LhsRep, LhsExponent, Radix>;
            using rhs_type = fixed_point<decltype(std::declval<RhsRep>()<<constant<shiftage>()), LhsExponent, Radix>;
            using operator_type = comparison_operator<Operator, lhs_type, rhs_type>;

            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs,
                    fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(operator_type{}(lhs, rhs))
            {
                return operator_type{}(lhs, rhs);
            }
        };

        template<typename Operator, typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct comparison_operator<Operator,
                fixed_point<LhsRep, LhsExponent, Radix>,
                fixed_point<RhsRep, RhsExponent, Radix>,
                enable_if_t<(RhsExponent<LhsExponent)>> {
            static constexpr int shiftage = LhsExponent - RhsExponent;
            using lhs_type = fixed_point<decltype(std::declval<LhsRep>()<<constant<shiftage>()), RhsExponent, Radix>;
            using rhs_type = fixed_point<RhsRep, RhsExponent, Radix>;
            using operator_type = comparison_operator<Operator, lhs_type, rhs_type>;

            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs,
                    fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(operator_type{}(lhs, rhs))
            {
                return operator_type{}(lhs, rhs);
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // multiply

        template<typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct binary_operator<multiply_op, fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs,
                    fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<
                    decltype(_impl::to_rep(lhs)*_impl::to_rep(rhs)),
                    LhsExponent+RhsExponent,
                    Radix>>(_impl::to_rep(lhs)*_impl::to_rep(rhs)))
            {
                return from_rep<fixed_point<
                        decltype(_impl::to_rep(lhs)*_impl::to_rep(rhs)),
                        LhsExponent+RhsExponent,
                        Radix>>(_impl::to_rep(lhs)*_impl::to_rep(rhs));
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // divide

        template<typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct binary_operator<divide_op, fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs,
                    fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<
                    decltype(_impl::to_rep(lhs)/_impl::to_rep(rhs)),
                    LhsExponent-RhsExponent,
                    Radix>>(_impl::to_rep(lhs)/_impl::to_rep(rhs)))
            {
                return from_rep<fixed_point<
                        decltype(_impl::to_rep(lhs)/_impl::to_rep(rhs)),
                        LhsExponent-RhsExponent,
                        Radix>>(_impl::to_rep(lhs)/_impl::to_rep(rhs));
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // modulo

        template<typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct binary_operator<modulo_op, fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs,
                    fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<
                    decltype(_impl::to_rep(lhs)%_impl::to_rep(rhs)),
                    LhsExponent,
                    Radix>>(_impl::to_rep(lhs)%_impl::to_rep(rhs)))
            {
                return from_rep<fixed_point<
                        decltype(_impl::to_rep(lhs)%_impl::to_rep(rhs)),
                        LhsExponent,
                        Radix>>(_impl::to_rep(lhs)%_impl::to_rep(rhs));
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // arithmetic operators that are degree 0 monomial

        template<class Operator> struct is_zero_degree : std::true_type {};
        template<> struct is_zero_degree<multiply_op> : std::false_type {};
        template<> struct is_zero_degree<divide_op> : std::false_type {};
        template<> struct is_zero_degree<modulo_op> : std::false_type {};
        template<> struct is_zero_degree<shift_left_op> : std::false_type {};
        template<> struct is_zero_degree<shift_right_op> : std::false_type {};

        // performs zero-degree binary operations between fixed_point types with the same exponent
        template<class Operator, class LhsRep, class RhsRep, int Exponent, int Radix>
        struct binary_operator<Operator, fixed_point<LhsRep, Exponent, Radix>, fixed_point<RhsRep, Exponent, Radix>,
                enable_if_t<is_zero_degree<Operator>::value>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, Exponent, Radix> const& lhs, fixed_point<RhsRep, Exponent, Radix> const& rhs) const
            -> decltype(from_rep<fixed_point<
                    decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs))),
                    Exponent,
                    Radix>>(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs))))
            {
                return from_rep<fixed_point<
                        decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs))),
                        Exponent,
                        Radix>>(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)));
            }
        };

        // performs zero-degree binary operations between fixed_point types with the different exponents
        template<typename Operator, typename LhsRep, int LhsExponent, typename RhsRep, int RhsExponent, int Radix>
        struct binary_operator<Operator, fixed_point<LhsRep, LhsExponent, Radix>, fixed_point<RhsRep, RhsExponent, Radix>,
                        enable_if_t<is_zero_degree<Operator>::value>> {
        private:
            static constexpr int _common_exponent = min(LhsExponent, RhsExponent);
            static constexpr int _lhs_left_shift = LhsExponent-_common_exponent;
            static constexpr int _rhs_left_shift = RhsExponent-_common_exponent;
        public:
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent, Radix> const& lhs, fixed_point<RhsRep, RhsExponent, Radix> const& rhs) const
            -> decltype(Operator{}(
                    from_rep<fixed_point<LhsRep, _common_exponent, Radix>>(
                            _impl::scale<_lhs_left_shift, Radix>(_impl::to_rep(lhs))),
                    from_rep<fixed_point<RhsRep, _common_exponent, Radix>>(
                            _impl::scale<_rhs_left_shift, Radix>(_impl::to_rep(rhs)))))
            {
                return Operator{}(
                        from_rep<fixed_point<LhsRep, _common_exponent, Radix>>(
                                _impl::scale<_lhs_left_shift, Radix>(_impl::to_rep(lhs))),
                        from_rep<fixed_point<RhsRep, _common_exponent, Radix>>(
                                _impl::scale<_rhs_left_shift, Radix>(_impl::to_rep(rhs))));
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // pre-increment/decrement arithmetic operators

        template<typename Operator, typename Rep, int Exponent, int Radix>
        struct pre_operator<Operator, fixed_point<Rep, Exponent, Radix>> {
            constexpr auto operator()(fixed_point<Rep, Exponent, Radix>& rhs) const
            -> decltype(typename pre_to_assign<Operator>::type{}(rhs, 1))
            {
                return typename pre_to_assign<Operator>::type{}(rhs, 1);
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // post-increment/decrement arithmetic operators

        template<typename Operator, typename Rep, int Exponent, int Radix>
        struct post_operator<Operator, fixed_point<Rep, Exponent, Radix>> {
            CNL_RELAXED_CONSTEXPR fixed_point<Rep, Exponent, Radix> operator()(fixed_point<Rep, Exponent, Radix>& rhs) const
            {
                auto copy = rhs;
                typename post_to_assign<Operator>::type{}(rhs, 1);
                return copy;
            }
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // shift operators

    // fixed_point, dynamic
    template<typename LhsRep, int LhsExponent, int LhsRadix, typename Rhs>
    constexpr auto operator<<(fixed_point<LhsRep, LhsExponent, LhsRadix> const& lhs, Rhs const& rhs)
    -> decltype(_impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) << int(rhs)), LhsExponent, LhsRadix>>(
            _impl::to_rep(lhs) << int(rhs)))
    {
        return _impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) << int(rhs)), LhsExponent, LhsRadix>>(
                _impl::to_rep(lhs) << int(rhs));
    }

    template<typename LhsRep, int LhsExponent, int LhsRadix, typename Rhs>
    constexpr auto operator>>(fixed_point<LhsRep, LhsExponent, LhsRadix> const& lhs, Rhs const& rhs)
    -> decltype(_impl::from_rep<fixed_point<
            decltype(_impl::to_rep(lhs) >> int(rhs)),
            LhsExponent,
            LhsRadix>>(_impl::to_rep(lhs) >> int(rhs)))
    {
        return _impl::from_rep<fixed_point<
                decltype(_impl::to_rep(lhs) >> int(rhs)),
                LhsExponent,
                LhsRadix>>(_impl::to_rep(lhs) >> int(rhs));
    }

    // fixed_point, const_integer
    template<typename LhsRep, int LhsExponent, int LhsRadix, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent+static_cast<int>(RhsValue), LhsRadix>
    operator<<(fixed_point<LhsRep, LhsExponent, LhsRadix> const& lhs, constant<RhsValue>)
    {
        return _impl::from_rep<fixed_point<LhsRep, LhsExponent+static_cast<int>(RhsValue), LhsRadix>>(
                _impl::to_rep(lhs));
    }

    template<typename LhsRep, int LhsExponent, int LhsRadix, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent-static_cast<int>(RhsValue), LhsRadix>
    operator>>(fixed_point<LhsRep, LhsExponent, LhsRadix> const& lhs, constant<RhsValue>)
    {
        return _impl::from_rep<fixed_point<LhsRep, LhsExponent-static_cast<int>(RhsValue), LhsRadix>>(
                _impl::to_rep(lhs));
    }
}

#endif  // CNL_FIXED_POINT_OPERATORS_H
