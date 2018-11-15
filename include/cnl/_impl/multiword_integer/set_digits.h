
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_MULTIWORD_INTEGER_SET_DIGITS_H)
#define CNL_IMPL_MULTIWORD_INTEGER_SET_DIGITS_H 1

#include "forward_declaration.h"
#include "optimal_multiword_integer.h"
#include "../num_traits/set_digits.h"

/// compositional numeric library
namespace cnl {
    template<typename Word, int NumWords, int Digits>
    struct set_digits<_impl::multiword_integer<Word, NumWords>, Digits>
            : _impl::optimal_multiword_integer<Digits, Word> {
        // TODO: Narrowest isn't the same as Word. How does this affect things?
    };
}

#endif  // CNL_IMPL_MULTIWORD_INTEGER_SET_DIGITS_H
