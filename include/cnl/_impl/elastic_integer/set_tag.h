
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_SET_TAG_H)
#define CNL_IMPL_ELASTIC_INTEGER_SET_TAG_H

#include "../num_traits/adopt_width.h"
#include "../num_traits/set_tag.h"
#include "declaration.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    /// \brief \ref elastic_integer specialization of \ref set_tag
    /// \headerfile cnl/elastic_integer.h
    template<int Digits, typename Narrowest, int NewDigits, typename NewNarrowest>
    struct set_tag<elastic_integer<Digits, Narrowest>, elastic_tag<NewDigits, NewNarrowest>>
        : std::type_identity<
                  elastic_integer<NewDigits, _impl::adopt_width_t<NewNarrowest, Narrowest>>> {
    };
}

#endif  // CNL_IMPL_ELASTIC_INTEGER_SET_TAG_H
