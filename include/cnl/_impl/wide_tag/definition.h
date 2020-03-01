
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_TAG_DEFINITION_H)
#define CNL_IMPL_WIDE_TAG_DEFINITION_H

#include "../common.h"
#include "../duplex_integer/instantiate_duplex_integer.h"
#include "../num_traits/digits.h"
#include "../num_traits/max_digits.h"
#include "../num_traits/set_digits.h"
#include "../operators/homogeneous_operator_tag_base.h"
#include "../type_traits/enable_if.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    /// \private specialization for when number can be represented in a single integer
    template<int Digits, typename Narrowest>
    struct wide_tag<
            Digits,
            Narrowest,
            _impl::enable_if_t<(Digits <= _impl::max_digits<Narrowest>::value)>>
        : _impl::homogeneous_operator_tag_base {
        using _rep = set_digits_t<
                Narrowest,
                _impl::max(cnl::digits<Narrowest>::value, Digits)>;
    };

    /// \private specialization for when number must be represented using multiple integers
    template<int Digits, typename Narrowest>
    struct wide_tag<
            Digits,
            Narrowest,
            _impl::enable_if_t<(_impl::max_digits<Narrowest>::value<Digits)>>
        : _impl::homogeneous_operator_tag_base {
        using _rep = _impl::instantiate_duplex_integer_t<Digits, Narrowest>;
    };
}

#endif  // CNL_IMPL_WIDE_TAG_DEFINITION_H
