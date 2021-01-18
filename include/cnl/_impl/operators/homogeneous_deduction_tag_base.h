
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OPERATORS_IS_HOMOGENEOUS_DEDUCTION_TAG_H)
#define CNL_IMPL_OPERATORS_IS_HOMOGENEOUS_DEDUCTION_TAG_H

#include "../type_traits/enable_if.h"
#include "custom_operator.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // a tag with unremarkable properties WRT deduction,
        // i.e. deduction from numbers with this tag
        // produce more numbers with the same tag
        struct homogeneous_deduction_tag_base {
        };

        template<tag Tag>
        struct is_homogeneous_deduction_tag : std::is_base_of<homogeneous_deduction_tag_base, Tag> {
        };
    }

    template<class ArchetypeTag, typename Initializer>
    requires _impl::is_homogeneous_deduction_tag<ArchetypeTag>::value struct deduction<ArchetypeTag, Initializer> {
        // tag associated with deduced type
        using tag = ArchetypeTag;

        // deduced type; equivalent to rep type of a component
        using type = Initializer;
    };
}

#endif  // CNL_IMPL_OPERATORS_IS_HOMOGENEOUS_DEDUCTION_TAG_H
