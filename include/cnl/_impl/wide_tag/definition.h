
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_TAG_DEFINITION_H)
#define CNL_IMPL_WIDE_TAG_DEFINITION_H

#include "../custom_operator/homogeneous_operator_tag_base.h"
#include "../duplex_integer/narrowest_integer.h"
#include "../num_traits/digits.h"
#include "../num_traits/max_digits.h"
#include "../num_traits/set_digits.h"
#include "declaration.h"

#include <algorithm>
#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<int Digits, typename Narrowest, bool NeedsDuplex>
        struct wide_tag_rep;

        // When number can be represented in a single integer
        template<int Digits, typename Narrowest>
        struct wide_tag_rep<Digits, Narrowest, false>
            : std::type_identity<set_digits_t<Narrowest, std::max(cnl::digits<Narrowest>, Digits)>> {
        };

        // when number must be represented using multiple integers
        template<int Digits, typename Narrowest>
        struct wide_tag_rep<Digits, Narrowest, true>
            : std::type_identity<narrowest_integer_t<Digits, Narrowest>> {
        };

        template<int Digits, typename Narrowest, bool NeedsDuplex>
        using wide_tag_rep_t = typename wide_tag_rep<Digits, Narrowest, NeedsDuplex>::type;
    }

    template<int Digits, typename Narrowest>
    struct wide_tag : _impl::homogeneous_operator_tag_base {
        using rep = _impl::wide_tag_rep_t<Digits, Narrowest, (Digits > _impl::max_digits<Narrowest>)>;
    };
}

#endif  // CNL_IMPL_WIDE_TAG_DEFINITION_H
