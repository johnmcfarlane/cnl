
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_TAG_DEFINITION_H)
#define CNL_IMPL_ELASTIC_TAG_DEFINITION_H

#include "../common.h"
#include "../num_traits/digits.h"
#include "../num_traits/set_digits.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, typename Narrowest>
    struct elastic_tag {
        // A remarkable property of the elastic_tag is that it's rep type is dependent upon it.
        using _rep = set_digits_t<Narrowest, _impl::max(cnl::digits<Narrowest>::value, Digits)>;
    };
}

#endif // CNL_IMPL_ELASTIC_TAG_DEFINITION_H
