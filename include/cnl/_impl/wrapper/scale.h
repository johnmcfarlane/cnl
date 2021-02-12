
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_SCALE_H)
#define CNL_IMPL_WRAPPER_SCALE_H

#include "../num_traits/scale.h"
#include "../operators/native_tag.h"
#include "definition.h"
#include "digits.h"
#include "operators.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, int Radix, typename Rep>
    struct scale<Digits, Radix, _impl::wrapper<Rep>> {
        using _value_type = _impl::wrapper<Rep>;
        [[nodiscard]] constexpr auto operator()(_value_type const& s) const
        {
            return _impl::from_rep<_value_type>(_impl::scale<Digits, Radix>(_impl::to_rep(s)));
        }
    };
}

#endif  // CNL_IMPL_WRAPPER_SCALE_H
