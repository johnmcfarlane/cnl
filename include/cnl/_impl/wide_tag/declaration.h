
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_TAG_DECLARATION_H)
#define CNL_IMPL_WIDE_TAG_DECLARATION_H

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<int Digits, typename Narrowest = int, class Enable = void>
    struct wide_tag;
}

#endif  // CNL_IMPL_WIDE_TAG_DECLARATION_H
