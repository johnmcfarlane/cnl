
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_NUMERIC_LIMITS_H)
#define CNL_IMPL_ELASTIC_INTEGER_NUMERIC_LIMITS_H

#include "../../numeric_limits.h"
#include "../limits/lowest.h"
#include "../num_traits/rep_of.h"
#include "../numbers/signedness.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    template<int Digits, class Narrowest>
    struct numeric_limits<elastic_integer<Digits, Narrowest>> : numeric_limits<Narrowest> {
    private:
        // elastic integer-specific helpers
        using value_type = elastic_integer<Digits, Narrowest>;
        using rep = _impl::rep_of_t<value_type>;
        using rep_numeric_limits = numeric_limits<rep>;

        [[nodiscard]] static constexpr auto rep_max() noexcept
        {
            return static_cast<rep>(
                    rep_numeric_limits::max() >> (rep_numeric_limits::digits - digits));
        }

    public:
        // standard members
        static constexpr int digits = Digits;

        [[nodiscard]] static constexpr auto min() noexcept
        {
            return value_type{1};
        }

        [[nodiscard]] static constexpr auto max() noexcept
        {
            return value_type{rep_max()};
        }

        [[nodiscard]] static constexpr auto lowest() noexcept
        {
            return value_type{_impl::lowest<rep, numbers::signedness_v<Narrowest>>()(rep_max())};
        }
    };

    template<int Digits, class Narrowest>
    struct numeric_limits<elastic_integer<Digits, Narrowest> const>
        : numeric_limits<elastic_integer<Digits, Narrowest>> {
    };
}

#endif  // CNL_IMPL_ELASTIC_INTEGER_NUMERIC_LIMITS_H
