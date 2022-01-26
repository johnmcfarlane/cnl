
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_NUMERIC_LIMITS_H)
#define CNL_IMPL_WRAPPER_NUMERIC_LIMITS_H

#include "../custom_operator/tag.h"
#include "definition.h"
#include "from_rep.h"

#include <limits>

/// compositional numeric library
namespace std {
    template<typename Rep, cnl::tag Tag>
    struct numeric_limits<cnl::_impl::wrapper<Rep, Tag>> : numeric_limits<Rep> {
    private:
        using value_type = cnl::_impl::wrapper<Rep, Tag>;
        using rep_numeric_limits = numeric_limits<Rep>;

    public:
        [[nodiscard]] static constexpr auto min()
        {
            return cnl::_impl::from_rep<value_type>(rep_numeric_limits::min());
        }

        [[nodiscard]] static constexpr auto max()
        {
            return cnl::_impl::from_rep<value_type>(rep_numeric_limits::max());
        }

        [[nodiscard]] static constexpr auto lowest()
        {
            return cnl::_impl::from_rep<value_type>(rep_numeric_limits::lowest());
        }

        [[nodiscard]] static constexpr auto epsilon() noexcept
        {
            return cnl::_impl::from_rep<value_type>(rep_numeric_limits::round_error());
        }

        [[nodiscard]] static constexpr auto round_error() noexcept
        {
            return static_cast<value_type>(rep_numeric_limits::round_error());
        }

        [[nodiscard]] static constexpr auto infinity()
        {
            return static_cast<value_type>(rep_numeric_limits::infinity());
        }

        [[nodiscard]] static constexpr auto
        quiet_NaN() noexcept  // NOLINT(readability-identifier-naming)
        {
            return static_cast<value_type>(rep_numeric_limits::quiet_NaN());
        }

        [[nodiscard]] static constexpr auto
        signaling_NaN() noexcept  // NOLINT(readability-identifier-naming)
        {
            return static_cast<value_type>(rep_numeric_limits::signaling_NaN());
        }

        [[nodiscard]] static constexpr auto denorm_min() noexcept
        {
            return static_cast<value_type>(rep_numeric_limits::denorm_min());
        }
    };

    template<typename Rep, cnl::tag Tag>
    struct numeric_limits<cnl::_impl::wrapper<Rep, Tag> const> : numeric_limits<cnl::_impl::wrapper<Rep, Tag>> {
    };
}

#endif  // CNL_IMPL_WRAPPER_NUMERIC_LIMITS_H
