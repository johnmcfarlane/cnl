
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_TAG_H)
#define CNL_IMPL_ELASTIC_INTEGER_TAG_H

#include "../elastic_tag.h"
#include "../num_traits/tag.h"
#include "../type_traits/type_identity.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    /// \brief \ref elastic_integer specialization of \ref tag
    /// \headerfile cnl/elastic_integer.h
    template<int Digits, typename Narrowest>
    struct tag<elastic_integer<Digits, Narrowest>>
            : _impl::type_identity<elastic_tag<Digits, Narrowest>> {
    };
}

#endif // CNL_IMPL_ELASTIC_INTEGER_TAG_H
