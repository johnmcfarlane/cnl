
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief specialisation of std::numeric_limits of scaled_integer

#if !defined(CNL_IMPL_SCALED_INTEGER_NUMERIC_LIMITS_H)
#define CNL_IMPL_SCALED_INTEGER_NUMERIC_LIMITS_H

#include "definition.h"
#include "from_rep.h"

#include <limits>

/// compositional numeric library
namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits of cnl::scaled_integer

    // note: some members are guessed,
    // some are temporary (assuming rounding style, traps etc.)
    // and some are undefined
    template<typename Rep, int Exponent, int Radix>
    struct numeric_limits<cnl::scaled_integer<Rep, cnl::power<Exponent, Radix>>>
        : numeric_limits<Rep> {
    private:
        using value_type = cnl::scaled_integer<Rep, cnl::power<Exponent, Radix>>;
        using rep_numeric_limits = numeric_limits<Rep>;

    public:
        // standard members

        [[nodiscard]] static constexpr auto min() noexcept
        {
            return cnl::_impl::from_rep<value_type>(Rep{1});
        }

        [[nodiscard]] static constexpr auto max() noexcept
        {
            return cnl::_impl::from_rep<value_type>(rep_numeric_limits::max());
        }

        [[nodiscard]] static constexpr auto lowest() noexcept
        {
            return cnl::_impl::from_rep<value_type>(rep_numeric_limits::lowest());
        }

        static constexpr bool is_specialized = true;

        static constexpr bool is_integer = false;

        [[nodiscard]] static constexpr auto epsilon() noexcept
        {
            return cnl::_impl::from_rep<value_type>(Rep{1});
        }

        [[nodiscard]] static constexpr auto round_error() noexcept
        {
            return cnl::_impl::from_rep<value_type>(Rep{0});
        }

        [[nodiscard]] static constexpr auto infinity() noexcept
        {
            return cnl::_impl::from_rep<value_type>(Rep{0});
        }

        [[nodiscard]] static constexpr auto
        quiet_NaN() noexcept  // NOLINT(readability-identifier-naming)
        {
            return cnl::_impl::from_rep<value_type>(Rep{0});
        }

        [[nodiscard]] static constexpr auto
        signaling_NaN() noexcept  // NOLINT(readability-identifier-naming)
        {
            return cnl::_impl::from_rep<value_type>(Rep{0});
        }

        [[nodiscard]] static constexpr auto denorm_min() noexcept
        {
            return cnl::_impl::from_rep<value_type>(Rep{1});
        }
    };
}

#endif  // CNL_IMPL_SCALED_INTEGER_NUMERIC_LIMITS_H
