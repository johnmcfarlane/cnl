
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_NUMERIC_LIMITS_H)
#define CNL_IMPL_WIDE_INTEGER_NUMERIC_LIMITS_H

#include "../../limits.h"
#include "../duplex_integer.h"
#include "../limits/lowest.h"
#include "../num_traits/rep.h"
#include "definition.h"
#include "from_rep.h"

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::numeric_limits specialization for overflow_integer

    template<int Digits, typename Narrowest>
    struct numeric_limits<wide_integer<Digits, Narrowest>>
            : numeric_limits<_impl::rep_t<wide_integer<Digits, Narrowest>>> {
        static constexpr bool is_integer = true;
        // wide_integer-specific helpers
        using _narrowest_numeric_limits = numeric_limits<Narrowest>;
        using _value_type = wide_integer<Digits, Narrowest>;
        using _rep = _impl::rep_t<_value_type>;
        using _rep_numeric_limits = numeric_limits<_rep>;

        // standard members
        static constexpr int digits = Digits;

        CNL_NODISCARD static constexpr _value_type min() noexcept
        {
            return _impl::from_rep<_value_type>(1);
        }

        CNL_NODISCARD static constexpr _value_type max() noexcept
        {
            return static_cast<_rep>(_rep_numeric_limits::max() >> (_rep_numeric_limits::digits-digits));
        }

        CNL_NODISCARD static constexpr _value_type lowest() noexcept
        {
            return static_cast<_rep>(_rep_numeric_limits::lowest() >> (_rep_numeric_limits::digits-digits));
        }
    };

    template<int Digits, typename Narrowest>
    struct numeric_limits<wide_integer<Digits, Narrowest> const>
            : numeric_limits<wide_integer<Digits, Narrowest>> {
        static constexpr bool is_integer = true;
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_NUMERIC_LIMITS_H
