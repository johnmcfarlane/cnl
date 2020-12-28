
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OVERFLOW_IS_OVERFLOW_TAG_H)
#define CNL_IMPL_OVERFLOW_IS_OVERFLOW_TAG_H

#include "../operators/is_same_tag_family.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Tag>
        struct is_overflow_tag : std::false_type {
        };
    }

    template<class T>
    concept overflow_tag = _impl::is_overflow_tag<T>::value;

    namespace _impl {
        template<overflow_tag Tag1, overflow_tag Tag2>
        struct is_same_tag_family<Tag1, Tag2>
            : std::true_type {
        };
    }
}

#endif  // CNL_IMPL_OVERFLOW_IS_OVERFLOW_TAG_H
