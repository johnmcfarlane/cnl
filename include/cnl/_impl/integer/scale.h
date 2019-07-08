
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_SCALE_H)
#define CNL_IMPL_INTEGER_SCALE_H

#include "digits.h"
#include "operators.h"
#include "type.h"
#include "../num_traits/scale.h"
#include "../operators/native_tag.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, int Radix, typename Rep>
    struct scale<Digits, Radix, _impl::integer<Rep>> {
        using _value_type = _impl::integer<Rep>;
        CNL_NODISCARD constexpr auto operator()(_value_type const& s) const
        -> decltype(_impl::from_rep<_value_type>(_impl::scale<Digits, Radix>(_impl::to_rep(s))))
        {
            return _impl::default_scale<Digits, Radix, _value_type>{}(s);
        }
    };
}

#endif  // CNL_IMPL_INTEGER_SCALE_H
