
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_NUMERIC_LIMITS_H)
#define CNL_IMPL_DUPLEX_INTEGER_NUMERIC_LIMITS_H

#include "../../limits.h"
#include "../num_traits/from_rep.h"
#include "ctors.h"
#include "forward_declaration.h"

/// compositional numeric library
namespace cnl {
    template<typename Upper, typename Lower>
    struct numeric_limits<_impl::duplex_integer<Upper, Lower>>
            : numeric_limits<Upper> {
        static constexpr bool is_integer = true;
        using _lower_numeric_limits = numeric_limits<Lower>;
        using _upper_numeric_limits = numeric_limits<Upper>;
        using _value_type = _impl::duplex_integer<Upper, Lower>;

        // standard members
        static constexpr int digits = _lower_numeric_limits::digits+_upper_numeric_limits::digits;

        static constexpr _value_type lowest() noexcept
        {
            return _value_type{
                    numeric_limits<Upper>::lowest(),
                    numeric_limits<Lower>::lowest()};
        }

        static constexpr _value_type min() noexcept
        {
            return _value_type{
                    numeric_limits<Upper>::min(),
                    numeric_limits<Lower>::min()};
        }

        static constexpr _value_type max() noexcept
        {
            return _value_type{
                    numeric_limits<Upper>::max(),
                    numeric_limits<Lower>::max()};
        }
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_NUMERIC_LIMITS_H
