
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_SET_REP_H)
#define CNL_IMPL_WIDE_INTEGER_SET_REP_H

#include "../num_traits/adopt_signedness.h"
#include "../num_traits/digits.h"
#include "../num_traits/set_rep.h"
#include "../number/declaration.h"
#include "../type_traits/type_identity.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    template<typename NumberRep, int NumberTagDigits, typename NumberTagNarrowest, typename Rep>
    struct set_rep<_impl::number<NumberRep, wide_tag<NumberTagDigits, NumberTagNarrowest>>, Rep>
        : _impl::type_identity<wide_integer<
                  NumberTagDigits, _impl::adopt_signedness_t<NumberTagNarrowest, Rep>>> {
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_SET_REP_H
