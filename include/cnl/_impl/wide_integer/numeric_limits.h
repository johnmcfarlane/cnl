
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WIDE_INTEGER_NUMERIC_LIMITS_H)
#define CNL_IMPL_WIDE_INTEGER_NUMERIC_LIMITS_H

#include "../../numeric_limits.h"
#include "../limits/lowest.h"
#include "../num_traits/rep_of.h"
#include "definition.h"
#include "from_rep.h"

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::numeric_limits specialization for overflow_integer

    template<int Digits, typename Narrowest>
    struct numeric_limits<wide_integer<Digits, Narrowest>>
        : numeric_limits<_impl::rep_of_t<wide_integer<Digits, Narrowest>>> {
    private:
        // wide_integer-specific helpers
        using value_type = wide_integer<Digits, Narrowest>;
        using rep = _impl::rep_of_t<value_type>;
        using rep_numeric_limits = numeric_limits<rep>;
        static_assert(rep_numeric_limits::is_integer);

    public:
        // standard members
        static constexpr int digits = Digits;

        [[nodiscard]] static constexpr auto min() noexcept
        {
            return value_type{1};
        }

        [[nodiscard]] static constexpr auto max() noexcept
        {
            return value_type{
                    rep_numeric_limits::max() >> (rep_numeric_limits::digits - digits)};
        }

        [[nodiscard]] static constexpr auto lowest() noexcept
        {
            return value_type{
                    rep_numeric_limits::lowest() >> (rep_numeric_limits::digits - digits)};
        }
    };

    template<int Digits, typename Narrowest>
    struct numeric_limits<wide_integer<Digits, Narrowest> const>
        : numeric_limits<wide_integer<Digits, Narrowest>> {
        static constexpr bool is_integer = true;
    };
}

#endif  // CNL_IMPL_WIDE_INTEGER_NUMERIC_LIMITS_H
