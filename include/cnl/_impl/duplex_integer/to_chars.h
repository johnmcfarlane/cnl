
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_TO_CHARS_H)
#define CNL_IMPL_DUPLEX_INTEGER_TO_CHARS_H

#include "../num_traits/set_rounding.h"
#include "../to_chars.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Upper, typename Lower>
        to_chars_result to_chars_positive(
                char* const first, char* const last, duplex_integer<Upper, Lower> const& value) noexcept
        {
            auto const natural_last = to_chars_natural(first, last, value);
            return to_chars_result{natural_last, natural_last?std::errc{}:std::errc::value_too_large};
        }

        // partial implementation of std::to_chars overloaded on cnl::duplex_integer
        template<typename Upper, typename Lower>
        to_chars_result to_chars(
                char* const first,
                char* const last,  // NOLINT(readability-non-const-parameter)
                _impl::duplex_integer<Upper, Lower> const& value)
        {
            if (!value) {
                if (first==last) {
                    // buffer too small to contain "0"
                    return to_chars_result{last, std::errc::value_too_large};
                }

                // zero
                *first = '0';
                return to_chars_result{first+1, std::errc{}};
            }

            using native_rounding_type = set_rounding_t<decltype(value), native_rounding_tag>;
            auto const& native_rounding_value = static_cast<native_rounding_type>(value);

            return _impl::to_chars_non_zero<native_rounding_type>{}(
                    first, last, native_rounding_value);
        }
    }

    using _impl::to_chars;
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_TO_CHARS_H
