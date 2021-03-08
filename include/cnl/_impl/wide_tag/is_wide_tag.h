
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_TAG_IS_WIDE_TAG_H)
#define CNL_IMPL_WIDE_TAG_IS_WIDE_TAG_H

#include "../custom_operator/tag.h"
#include "declaration.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename T>
        inline constexpr auto is_wide_tag = false;

        template<int Digits, typename Narrowest>
        inline constexpr auto is_wide_tag<wide_tag<Digits, Narrowest>> = true;

        template<class T>
        concept any_wide_tag = is_tag<T>&& is_wide_tag<T>;
    }
}

#endif  // CNL_IMPL_WIDE_TAG_IS_WIDE_TAG_H
