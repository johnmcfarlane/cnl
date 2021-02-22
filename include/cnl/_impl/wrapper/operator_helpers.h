
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_OPERATOR_HELPERS_H)
#define CNL_IMPL_WRAPPER_OPERATOR_HELPERS_H

#include "../operators/custom_operator.h"
#include "is_wrapper.h"
#include "rep_of.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::wants_generic_ops<number<>>

        template<_impl::wrapped Number>
        inline constexpr auto wants_generic_ops<Number> = true;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::composition_depth

        template<class Number, bool IsComposite = is_composite<Number>::value>
        struct composition_depth;

        template<class Number>
        struct composition_depth<Number, true> {
            using _rep = _impl::rep_of_t<Number>;
            static constexpr auto value = composition_depth<_rep>::value + 1;
        };

        template<class T>
        struct composition_depth<T, false> : std::integral_constant<int, 0> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::can_be_number_wrapper

        template<typename Wrapper>
        inline constexpr auto can_be_number_wrapper = is_wrapper<Wrapper>;

        template<typename Wrapper, int WrapperN>
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays)
        inline constexpr auto can_be_number_wrapper<Wrapper[WrapperN]> = false;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::can_be_wrapped_by_number

        template<typename Rep>
        struct can_be_wrapped_by_number
            : std::integral_constant<
                      bool, cnl::numeric_limits<Rep>::is_specialized
                                    && !std::is_floating_point<Rep>::value> {
        };

        template<typename Rep, int RepN>
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays)
        struct can_be_wrapped_by_number<Rep[RepN]> : std::false_type {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_same_number_wrapper

        template<typename T1, typename T2>
        struct is_same_number_wrapper
            : std::integral_constant<
                      bool, std::is_same<from_value_t<T1, int>, from_value_t<T2, int>>::value> {
        };

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::number_can_wrap

        template<typename Wrapper, typename Rep>
        struct number_can_wrap
            : std::integral_constant<
                      bool, can_be_wrapped_by_number<Rep>::value
                                    && can_be_number_wrapper<Wrapper> && !is_same_number_wrapper<Wrapper, Rep>::value
                                    && (composition_depth<Rep>::value < composition_depth<Wrapper>::value)> {
        };
    }
}

#endif  // CNL_IMPL_WRAPPER_OPERATOR_HELPERS_H
