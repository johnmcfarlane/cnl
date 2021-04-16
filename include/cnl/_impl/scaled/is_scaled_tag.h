
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_IS_SCALED_TAG_H)
#define CNL_IMPL_SCALED_IS_SCALED_TAG_H

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<class Tag>
    struct is_scaled_tag : std::false_type {
    };

    template<class Tag>
    concept scaled_tag = is_scaled_tag<Tag>::value;
}

#endif  // CNL_IMPL_SCALED_IS_SCALED_TAG_H
