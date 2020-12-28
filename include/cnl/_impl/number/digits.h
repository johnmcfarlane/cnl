
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_DIGITS_H)
#define CNL_IMPL_NUMBER_DIGITS_H

#include "../num_traits/digits.h"
#include "../num_traits/tag.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, tag Tag>
    inline constexpr auto digits<_impl::number<Rep, Tag>> = digits<Rep>;
}

#endif  // CNL_IMPL_NUMBER_DIGITS_H
