
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OPERATORS_NATIVE_TAG_H)
#define CNL_IMPL_OPERATORS_NATIVE_TAG_H

#include "../../constant.h"
#include "../numbers/set_signedness.h"
#include "definition.h"
#include "homogeneous_deduction_tag_base.h"
#include "homogeneous_operator_tag_base.h"
#include "op.h"

#include <concepts>
#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // match the behavior of fundamental arithmetic types
        struct native_tag
            : homogeneous_deduction_tag_base
            , homogeneous_operator_tag_base {
            using identity = native_tag;
        };

        // true iff given type, T, provides its own operators
        template<typename T>
        struct has_native_operators
            : std::integral_constant<
                      bool, is_constant<T>::value || std::is_integral_v<T> || std::is_floating_point<T>::value> {
        };
    }

    template<>
    inline constexpr auto is_tag<_impl::native_tag> = true;

    template<typename Source, typename Destination>
    struct custom_operator<_impl::convert_op, op_value<Source>, op_value<Destination>> {
        [[nodiscard]] constexpr auto operator()(Source const& from) const -> Destination
        {
            return _impl::convert_op{}.template operator()<Destination>(from);
        }
    };

    template<_impl::unary_arithmetic_op Operator, typename Rhs>
    requires(_impl::has_native_operators<Rhs>::value) struct custom_operator<Operator, op_value<Rhs>> : Operator {
    };

    template<_impl::binary_op Operator, typename Lhs, typename Rhs>
    requires(_impl::has_native_operators<Lhs>::value&& _impl::has_native_operators<Rhs>::value) struct custom_operator<Operator, op_value<Lhs>, op_value<Rhs>> : Operator {
    };
}

#endif  // CNL_IMPL_OPERATORS_NATIVE_TAG_H
