
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OPERATORS_IS_HOMOGENEOUS_OPERATOR_TAG_H)
#define CNL_IMPL_OPERATORS_IS_HOMOGENEOUS_OPERATOR_TAG_H

#include "../config.h"
#include "../type_traits/enable_if.h"
#include "generic.h"
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

        template<tag Tag>
        struct is_homogeneous_operator_tag : std::is_base_of<homogeneous_operator_tag_base, Tag> {
        };

        // 'Boring' tags make use of the generic operator system.
        // For example, when you add two `rounding_nearest_tag` numbers together,
        // the result's tag is also `rounding_nearest_tag`.
        // But when you add two `elastic_integer_tag<N>` numbers together,
        // the result's tag is NOT also `elastic_integer_tag<N>`, <gosh>.
        template<tag Tag>
        inline constexpr auto
                wants_generic_ops<Tag, enable_if_t<is_homogeneous_operator_tag<Tag>::value>> = true;
    }

    template<_impl::binary_op Operator, tag Tag>
    struct binary_operator<
            Operator, _impl::native_tag, _impl::native_tag, Tag, Tag,
            _impl::enable_if_t<_impl::is_homogeneous_operator_tag<Tag>::value>> {
        CNL_NODISCARD constexpr Tag operator()(Tag, Tag) const
        {
            return Tag{};
        }
    };

    template<_impl::comparison_op Operator, tag Tag>
    struct comparison_operator<
            Operator, Tag, Tag,
            _impl::enable_if_t<_impl::is_homogeneous_operator_tag<Tag>::value>> {
        CNL_NODISCARD constexpr bool operator()(Tag, Tag) const
        {
            return true;
        }
    };
}

#endif  // CNL_IMPL_OPERATORS_IS_HOMOGENEOUS_OPERATOR_TAG_H
