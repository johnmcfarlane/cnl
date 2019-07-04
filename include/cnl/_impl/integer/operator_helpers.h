
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_OPERATOR_HELPERS_H)
#define CNL_IMPL_INTEGER_OPERATOR_HELPERS_H

#include "type.h"
#include "../operators/generic.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::wants_generic_ops<integer<>>

        template<typename Rep, class Tag>
        struct wants_generic_ops<integer<Rep, Tag>> : std::true_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::integer_depth

        template<class Wrapper, bool IsComposite = is_composite<Wrapper>::value>
        struct integer_depth;

        template<class Wrapper>
        struct integer_depth<Wrapper, true> {
            using _rep = _impl::to_rep_t<Wrapper>;
            static constexpr auto value = integer_depth<_rep>::value + 1;
        };

        template<class T>
        struct integer_depth<T, false> : std::integral_constant<int, 0> {};

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::can_be_integer_wrapper

        template<typename Wrapper>
        struct can_be_integer_wrapper : is_integer<Wrapper> {
        };

        template<typename Wrapper, int WrapperN>
        struct can_be_integer_wrapper<Wrapper[WrapperN]> : std::false_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::can_be_wrapped_by_integer

        template<typename Rep>
        struct can_be_wrapped_by_integer : std::integral_constant<bool,
                cnl::numeric_limits<Rep>::is_specialized && !std::is_floating_point<Rep>::value> {
        };

        template<typename Rep, int RepN>
        struct can_be_wrapped_by_integer<Rep[RepN]> : std::false_type {};

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_same_integer_wrapper

        template<typename T1, typename T2>
        struct is_same_integer_wrapper : std::integral_constant<bool,
                std::is_same<from_value_t<T1, int>, from_value_t<T2, int>>::value> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::integer_can_wrap

        template<typename Wrapper, typename Rep>
        struct integer_can_wrap
                : std::integral_constant<bool, can_be_wrapped_by_integer<Rep>::value
                        && can_be_integer_wrapper<Wrapper>::value
                        && !is_same_integer_wrapper<Wrapper, Rep>::value
                        && (integer_depth<Rep>::value < integer_depth<Wrapper>::value)> {};
    }
}

#endif  // CNL_IMPL_INTEGER_OPERATOR_HELPERS_H
