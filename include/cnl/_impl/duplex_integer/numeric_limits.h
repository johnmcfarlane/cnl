
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_NUMERIC_LIMITS_H)
#define CNL_IMPL_DUPLEX_INTEGER_NUMERIC_LIMITS_H

#include "../../numeric_limits.h"
#include "../num_traits/from_rep.h"
#include "ctors.h"
#include "declaration.h"

/// compositional numeric library
namespace cnl {
    template<typename Upper, typename Lower>
    struct numeric_limits<_impl::duplex_integer<Upper, Lower>> : numeric_limits<Upper> {
    private:
        using lower_numeric_limits = numeric_limits<Lower>;
        static_assert(lower_numeric_limits::is_integer);

        using upper_numeric_limits = numeric_limits<Upper>;
        static_assert(upper_numeric_limits::is_integer);

        using value_type = _impl::duplex_integer<Upper, Lower>;

    public:
        // standard members
        static constexpr int digits = lower_numeric_limits::digits + upper_numeric_limits::digits;

        [[nodiscard]] static constexpr auto lowest() noexcept
        {
            return value_type{numeric_limits<Upper>::lowest(), numeric_limits<Lower>::lowest()};
        }

        [[nodiscard]] static constexpr auto min() noexcept
        {
            return value_type{numeric_limits<Upper>::min(), numeric_limits<Lower>::min()};
        }

        [[nodiscard]] static constexpr auto max() noexcept
        {
            return value_type{numeric_limits<Upper>::max(), numeric_limits<Lower>::max()};
        }
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_NUMERIC_LIMITS_H
