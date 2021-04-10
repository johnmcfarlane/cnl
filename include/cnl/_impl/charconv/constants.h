
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_CHARCONV_CONSTANTS_H)
#define CNL_IMPL_CHARCONV_CONSTANTS_H

/// compositional numeric library
namespace cnl::_impl {
    constexpr auto radix_char{'.'};
    constexpr auto e_char{'e'};
    constexpr auto plus_char{'+'};
    constexpr auto minus_char{'-'};
    constexpr auto zero_char{'0'};

    constexpr auto isdigit(char c)
    {
        return c >= zero_char && c <= '9';
    }
}

#endif  // CNL_IMPL_CHARCONV_CONSTANTS_H
