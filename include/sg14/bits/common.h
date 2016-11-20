
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// definitions that are directly required by more than one header of the API

#if !defined(SG14_COMMON_H)
#define SG14_COMMON_H 1

#if ! defined(SG14_GODBOLT_ORG)
#include <sg14/type_traits>
#include <sg14/limits>
#endif

namespace sg14 {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::max

        template<class T>
        constexpr T max(T a, T b)
        {
            return (a<b) ? b : a;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::max

        template<class T>
        constexpr T min(T a, T b)
        {
            return (a<b) ? a : b;
        }

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::common_type_t

        // pre-C++14 common_type_t
        template<class ... T>
        using common_type_t = typename std::common_type<T ...>::type;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::identical - compiles iff same type; returns true iff equal

        template<typename A, typename B>
        constexpr bool identical(const A& a, const B& b)
        {
            static_assert(std::is_same<A, B>::value, "different types");
            return a==b;
        }
        
        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::is_integer_or_float - trait to identify 'traditional' arithmetic concept

        template<class T>
        struct is_integer_or_float : std::integral_constant<
                bool,
                std::numeric_limits<T>::is_integer || std::is_floating_point<T>::value> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // operator helpers

        ////////////////////////////////////////////////////////////////////////////////
        // operation tags

        struct negate_tag;

        struct add_tag;

        struct subtract_tag;

        struct multiply_tag;

        struct divide_tag;

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::op

        template<class OperationTag, class ... Operands>
        struct op;

        template<class Rhs>
        struct op<negate_tag, Rhs> {
            constexpr auto operator()(const Rhs& rhs) const -> decltype(-rhs)
            {
                return -rhs;
            }
        };

        template<class Lhs, class Rhs>
        struct op<add_tag, Lhs, Rhs> {
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs+rhs)
            {
                return lhs+rhs;
            }
        };

        template<class Lhs, class Rhs>
        struct op<subtract_tag, Lhs, Rhs> {
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs-rhs)
            {
                return lhs-rhs;
            }
        };

        template<class Lhs, class Rhs>
        struct op<multiply_tag, Lhs, Rhs> {
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs*rhs)
            {
                return lhs*rhs;
            }
        };

        template<class Lhs, class Rhs>
        struct op<divide_tag, Lhs, Rhs> {
            constexpr auto operator()(const Lhs& lhs, const Rhs& rhs) const -> decltype(lhs/rhs)
            {
                return lhs/rhs;
            }
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::rep_op_fn

        template<class OperationTag, class Lhs, class Rhs>
        constexpr auto op_fn(const Lhs& lhs, const Rhs& rhs)
        -> decltype(op<OperationTag, Lhs, Rhs>()(lhs, rhs))
        {
            return op<OperationTag, Lhs, Rhs>()(lhs, rhs);
        };

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::rep_op_result

        template<class OperationTag, class Lhs, class Rhs>
        using op_result = decltype(op_fn<OperationTag, Lhs, Rhs>(std::declval<Lhs>(), std::declval<Rhs>()));

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_impl::make_signed - std::make_signed with IsSigned parameter

        template<class T, bool IsSigned>
        struct make_signed;

        template<class T>
        struct make_signed<T, true> : sg14::make_signed<T> {
        };

        template<class T>
        struct make_signed<T, false> : sg14::make_unsigned<T> {
        };

        template<class T, bool IsSigned>
        using make_signed_t = typename make_signed<T, IsSigned>::type;
    }
}

#endif  // SG14_COMMON_H
