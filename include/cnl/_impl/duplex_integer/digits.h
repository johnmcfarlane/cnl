
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_DIGITS_H)
#define CNL_IMPL_DUPLEX_INTEGER_DIGITS_H

#include "forward_declaration.h"
#include "../num_traits/digits.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // 'sensible' shift operations - for when the compiler is being obtuse

        template<typename Lhs>
        constexpr auto sensible_right_shift(Lhs const& lhs, int rhs)
        -> decltype(lhs >> rhs)
        {
            return (rhs>=digits<Lhs>::value) ? Lhs{} >> 0 : lhs >> rhs;
        }

        template<typename Lhs>
        constexpr auto sensible_left_shift(Lhs const& lhs, int rhs)
        -> decltype(lhs << rhs)
        {
            using result_type = decltype(lhs << rhs);
            return (rhs>=digits<result_type>::value)
                   ? Lhs{} << 0
                   : ((is_signed<Lhs>::value && rhs>0) ? lhs & (numeric_limits<Lhs>::max() >> rhs) : lhs) << rhs;
        }

        template<typename Lhs>
        constexpr auto extra_sensible_right_shift(Lhs const& lhs, int rhs)
        -> decltype(lhs << -rhs)
        {
            return (rhs<0) ? sensible_left_shift(lhs, -rhs) : sensible_right_shift(lhs, rhs);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::digits<cnl::_impl::duplex_integer<>>

    template<typename Upper, typename Lower>
    struct digits<_impl::duplex_integer<Upper, Lower>>
            : std::integral_constant<int, digits<Upper>::value+digits<Lower>::value> {
    };
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_DIGITS_H
