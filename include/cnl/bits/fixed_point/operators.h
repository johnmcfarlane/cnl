
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief `cnl::fixed_point` operators

#if !defined(CNL_FIXED_POINT_OPERATORS_H)
#define CNL_FIXED_POINT_OPERATORS_H 1

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // heterogeneous operator overloads
    //
    // compare two objects of different fixed_point specializations

    namespace _fixed_point_operators_impl {
        template<class Lhs, class Rhs>
        constexpr bool is_heterogeneous() {
            return (!std::is_same<Lhs, Rhs>::value) &&
                   (_impl::is_fixed_point<Lhs>::value || _impl::is_fixed_point<Rhs>::value);
        }
    }

    namespace _impl {
        template<class Operator, class Rep, int Exponent>
        struct unary_operator<Operator, fixed_point<Rep, Exponent>> {
            constexpr auto operator()(fixed_point<Rep, Exponent> const& rhs) const
            -> decltype(from_rep<fixed_point<decltype(Operator()(to_rep(rhs))), Exponent>>(Operator()(to_rep(rhs))))
            {
                return from_rep<fixed_point<decltype(Operator()(to_rep(rhs))), Exponent>>(Operator()(to_rep(rhs)));
            }
        };

        template<class Operator, class Rep, int Exponent>
        struct comparison_operator<Operator, fixed_point<Rep, Exponent>, fixed_point<Rep, Exponent>> {
            constexpr auto operator()(
                    fixed_point<Rep, Exponent> const& lhs,
                    fixed_point<Rep, Exponent> const& rhs) const
            -> decltype(Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs)))
            {
                return Operator()(_impl::to_rep(lhs), _impl::to_rep(rhs));
            }
        };

        template<class Operator, class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
        struct comparison_operator<Operator, fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent> const& lhs,
                    fixed_point<RhsRep, RhsExponent> const& rhs) const
            -> decltype(Operator()(static_cast<_impl::common_type_t<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>>>(lhs), static_cast<_impl::common_type_t<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>>>(rhs)))
            {
                using common_type = _impl::common_type_t<fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>>;
                return Operator()(static_cast<common_type>(lhs), static_cast<common_type>(rhs));
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // multiply

        template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
        struct binary_operator<multiply_op, fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent> const& lhs,
                    fixed_point<RhsRep, RhsExponent> const& rhs) const
            -> decltype(from_rep<fixed_point<decltype(to_rep(lhs)*to_rep(rhs)), LhsExponent+RhsExponent>>(to_rep(lhs)*to_rep(rhs)))
            {
                return from_rep<fixed_point<decltype(to_rep(lhs)*to_rep(rhs)), LhsExponent+RhsExponent>>(to_rep(lhs)*to_rep(rhs));
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // divide

        template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
        struct binary_operator<divide_op, fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent> const& lhs,
                    fixed_point<RhsRep, RhsExponent> const& rhs) const
            -> decltype(from_rep<fixed_point<decltype(to_rep(lhs)/to_rep(rhs)), LhsExponent-RhsExponent>>(to_rep(lhs)/to_rep(rhs)))
            {
                return from_rep<fixed_point<decltype(to_rep(lhs)/to_rep(rhs)), LhsExponent-RhsExponent>>(to_rep(lhs)/to_rep(rhs));
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // modulo

        template<class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
        struct binary_operator<modulo_op, fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent> const& lhs,
                    fixed_point<RhsRep, RhsExponent> const& rhs) const
            -> decltype(from_rep<fixed_point<decltype(to_rep(lhs)%to_rep(rhs)), LhsExponent>>(to_rep(lhs)%to_rep(rhs)))
            {
                return from_rep<fixed_point<decltype(to_rep(lhs)%to_rep(rhs)), LhsExponent>>(to_rep(lhs)%to_rep(rhs));
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // add and subtract

        template<class Operator> struct is_add_or_subtract_op : std::false_type {};
        template<> struct is_add_or_subtract_op<add_op> : std::true_type {};
        template<> struct is_add_or_subtract_op<subtract_op> : std::true_type {};

        template<class Operator, class LhsRep, class RhsRep, int Exponent>
        struct binary_operator<Operator, fixed_point<LhsRep, Exponent>, fixed_point<RhsRep, Exponent>,
                enable_if_t<is_add_or_subtract_op<Operator>::value>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, Exponent> const& lhs, fixed_point<RhsRep, Exponent> const& rhs) const
            -> decltype(from_rep<fixed_point<decltype(Operator()(to_rep(lhs), to_rep(rhs))), Exponent>>(Operator()(to_rep(lhs), to_rep(rhs))))
            {
                return from_rep<fixed_point<decltype(Operator()(to_rep(lhs), to_rep(rhs))), Exponent>>(Operator()(to_rep(lhs), to_rep(rhs)));
            }
        };

        template<class Operator, class LhsRep, int LhsExponent, class RhsRep, int RhsExponent>
        struct binary_operator<Operator, fixed_point<LhsRep, LhsExponent>, fixed_point<RhsRep, RhsExponent>,
                enable_if_t<is_add_or_subtract_op<Operator>::value>> {
            constexpr auto operator()(
                    fixed_point<LhsRep, LhsExponent> const& lhs, fixed_point<RhsRep, RhsExponent> const& rhs) const
            -> decltype(binary_operator<Operator, fixed_point<LhsRep, min(LhsExponent, RhsExponent)>, fixed_point<RhsRep, min(LhsExponent, RhsExponent)>>()(lhs, rhs))
            {
                return binary_operator<Operator, fixed_point<LhsRep, min(LhsExponent, RhsExponent)>, fixed_point<RhsRep, min(LhsExponent, RhsExponent)>>()(lhs, rhs);
            }
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // shift operators

    // fixed_point, dynamic
    template<class LhsRep, int LhsExponent, class Rhs>
    constexpr auto operator<<(fixed_point<LhsRep, LhsExponent> const& lhs, Rhs const& rhs)
    -> decltype(_impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) << int(rhs)), LhsExponent>>(_impl::to_rep(lhs) << int(rhs)))
    {
        return _impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) << int(rhs)), LhsExponent>>(_impl::to_rep(lhs) << int(rhs));
    }

    template<class LhsRep, int LhsExponent, class Rhs>
    constexpr auto operator>>(fixed_point<LhsRep, LhsExponent> const& lhs, Rhs const& rhs)
    -> decltype(_impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) >> int(rhs)), LhsExponent>>(_impl::to_rep(lhs) >> int(rhs)))
    {
        return _impl::from_rep<fixed_point<decltype(_impl::to_rep(lhs) >> int(rhs)), LhsExponent>>(_impl::to_rep(lhs) >> int(rhs));
    }

    // fixed_point, const_integer
    template<class LhsRep, int LhsExponent, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent+RhsValue>
    operator<<(fixed_point<LhsRep, LhsExponent> const& lhs, constant<RhsValue>)
    {
        return _impl::from_rep<fixed_point<LhsRep, LhsExponent+RhsValue>>(_impl::to_rep(lhs));
    }

    template<class LhsRep, int LhsExponent, CNL_IMPL_CONSTANT_VALUE_TYPE RhsValue>
    constexpr fixed_point<LhsRep, LhsExponent-RhsValue>
    operator>>(fixed_point<LhsRep, LhsExponent> const& lhs, constant<RhsValue>)
    {
        return _impl::from_rep<fixed_point<LhsRep, LhsExponent-RhsValue>>(_impl::to_rep(lhs));
    }
}

#endif  // CNL_FIXED_POINT_OPERATORS_H
