
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_TAG_IS_ELASTIC_TAG_H)
#define CNL_IMPL_ELASTIC_TAG_IS_ELASTIC_TAG_H

#include "declaration.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename T>
        struct is_elastic_tag : std::false_type {
        };

        template<int Digits, typename Narrowest>
        struct is_elastic_tag<elastic_tag<Digits, Narrowest>> : std::true_type {
        };
    }
}

#endif //CNL_IMPL_ELASTIC_TAG_IS_ELASTIC_TAG_H
