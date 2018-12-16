
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_MULTIWORD_INTEGER_DIGITS_H)
#define CNL_IMPL_MULTIWORD_INTEGER_DIGITS_H 1

#include "forward_declaration.h"
#include "../num_traits/digits.h"
#include "../type_traits/set_signedness.h"

/// compositional numeric library
namespace cnl {
    template<typename Word, int NumWords>
    struct digits<
            _impl::multiword_integer<Word, NumWords>,
            _impl::enable_if_t<!is_signed<Word>::value>>
            : std::integral_constant<int, digits<Word>::value*NumWords> {
    };

    template<typename Word, int NumWords>
    struct digits<
            _impl::multiword_integer<Word, NumWords>,
            _impl::enable_if_t<is_signed<Word>::value>>
            : std::integral_constant<
                    int,
                    digits<_impl::multiword_integer<_impl::set_signedness_t<Word, false>, NumWords>>::value-1> {
    };
}

#endif  // CNL_IMPL_MULTIWORD_INTEGER_DIGITS_H
