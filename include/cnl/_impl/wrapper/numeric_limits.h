
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_NUMERIC_LIMITS_H)
#define CNL_IMPL_WRAPPER_NUMERIC_LIMITS_H

#include "../../limits.h"
#include "../num_traits/tag.h"
#include "definition.h"
#include "from_rep.h"

/// compositional numeric library
namespace cnl {
    template<typename Rep, tag Tag>
    struct numeric_limits<_impl::wrapper<Rep, Tag>> : cnl::numeric_limits<Rep> {
        using _value_type = _impl::wrapper<Rep, Tag>;
        using _rep_numeric_limits = numeric_limits<Rep>;

        CNL_NODISCARD static constexpr _value_type min() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::min());
        }

        CNL_NODISCARD static constexpr _value_type max() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::max());
        }

        CNL_NODISCARD static constexpr _value_type lowest() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::lowest());
        }

        CNL_NODISCARD static constexpr _value_type epsilon() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::round_error());
        }

        CNL_NODISCARD static constexpr _value_type round_error() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::round_error());
        }

        CNL_NODISCARD static constexpr _value_type infinity() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::infinity());
        }

        CNL_NODISCARD static constexpr _value_type
        quiet_NaN() noexcept  // NOLINT(readability-identifier-naming)
        {
            return static_cast<_value_type>(_rep_numeric_limits::quiet_NaN());
        }

        CNL_NODISCARD static constexpr _value_type
        signaling_NaN() noexcept  // NOLINT(readability-identifier-naming)
        {
            return static_cast<_value_type>(_rep_numeric_limits::signaling_NaN());
        }

        CNL_NODISCARD static constexpr _value_type denorm_min() noexcept
        {
            return static_cast<_value_type>(_rep_numeric_limits::denorm_min());
        }
    };

    template<typename Rep, tag Tag>
    struct numeric_limits<_impl::wrapper<Rep, Tag> const> : numeric_limits<_impl::wrapper<Rep, Tag>> {
    };
}

#endif  // CNL_IMPL_WRAPPER_NUMERIC_LIMITS_H
