
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_OPERATORS_TAG_H)
#define CNL_IMPL_OPERATORS_TAG_H

/// compositional numeric library
namespace cnl {
    template<class T>
    inline constexpr auto is_tag = false;

    template<class T>
    concept tag = is_tag<T>;
}

#endif  // CNL_IMPL_OPERATORS_TAG_H
