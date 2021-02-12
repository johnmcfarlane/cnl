
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OPERATORS_IS_HOMOGENEOUS_OPERATOR_TAG_H)
#define CNL_IMPL_OPERATORS_IS_HOMOGENEOUS_OPERATOR_TAG_H

#include "../config.h"
#include "custom_operator.h"
#include "overloads.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // a tag with unremarkable properties WRT operators,
        // i.e. operations on numbers with this tag
        // produce more numbers with the same tag
        struct homogeneous_operator_tag_base {
        };

        template<class Tag>
        concept homogeneous_operator_tag = tag<Tag>&& std::is_base_of_v<homogeneous_operator_tag_base, Tag>;

        // 'Boring' tags make use of the generic operator system.
        // For example, when you add two `rounding_nearest_tag` numbers together,
        // the result's tag is also `rounding_nearest_tag`.
        // But when you add two `elastic_integer_tag<N>` numbers together,
        // the result's tag is NOT also `elastic_integer_tag<N>`, <gosh>.
        template<homogeneous_operator_tag Tag>
        inline constexpr auto wants_generic_ops<Tag> = true;
    }

    template<_impl::binary_arithmetic_op Operator, _impl::homogeneous_operator_tag Tag>
    struct custom_operator<Operator, operand<Tag>, operand<Tag>> {
        [[nodiscard]] constexpr auto operator()(Tag, Tag) const
        {
            return Tag{};
        }
    };

    template<_impl::comparison_op Operator, _impl::homogeneous_operator_tag Tag>
    struct custom_operator<Operator, operand<Tag>, operand<Tag>> {
        [[nodiscard]] constexpr auto operator()(Tag, Tag) const
        {
            return true;
        }
    };
}

#endif  // CNL_IMPL_OPERATORS_IS_HOMOGENEOUS_OPERATOR_TAG_H
