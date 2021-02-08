
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_NUMBERS_H)
#define CNL_IMPL_WRAPPER_NUMBERS_H

#include "../../numbers.h"
#include "declaration.h"

#include <type_traits>

/// compositional numeric library, numbers header/namespace
namespace cnl::numbers {
    template<typename Rep, tag Tag>
    struct signedness<_impl::wrapper<Rep, Tag>> : signedness<Rep> {
    };

    template<typename Rep, tag Tag, bool IsSigned>
    struct set_signedness<_impl::wrapper<Rep, Tag>, IsSigned>
        : std::type_identity<_impl::wrapper<set_signedness_t<Rep, IsSigned>, Tag>> {
    };
}

#endif  // CNL_IMPL_WRAPPER_NUMBERS_H
