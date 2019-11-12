
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_DEFINITION_H)
#define CNL_IMPL_WIDE_INTEGER_DEFINITION_H

#include "../num_traits/digits.h"
#include "../number.h"
#include "../wide_tag.h"
#include "wide_integer_rep.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // wide_integer
        template<int Digits = digits<int>::value, typename Narrowest = int>
        using wide_integer = _impl::number<
                typename wide_tag<Digits, Narrowest>::_rep,
                wide_tag<Digits, Narrowest>>;
    }
}

#endif  // CNL_IMPL_WIDE_INTEGER_DEFINITION_H
