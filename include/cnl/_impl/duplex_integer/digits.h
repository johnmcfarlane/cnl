
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_DIGITS_H)
#define CNL_IMPL_DUPLEX_INTEGER_DIGITS_H

#include "../assert.h"
#include "../num_traits/digits.h"
#include "forward_declaration.h"
#include "remove_signedness.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // 'sensible' shift operations - for when the compiler is being obtuse

        template<typename Result, typename Lhs>
        CNL_NODISCARD constexpr auto sensible_right_shift(Lhs const& lhs, int rhs)
        -> enable_if_t<digits<Result>::value <= digits<decltype(lhs>>rhs)>::value, Result>
        {
            CNL_ASSERT(rhs>=0);
            using promoted_type = decltype(lhs >> rhs);
            return static_cast<Result>((rhs>=digits<promoted_type>::value)
                    // TODO: Not reproduced locally. Investigate.
                    // NOLINTNEXTLINE(clang-analyzer-core.UndefinedBinaryOperatorResult)
                   ? lhs >> (digits<Lhs>::value-1) >> 1
                    // TODO: Not reproduced locally. Investigate.
                    // NOLINTNEXTLINE(clang-analyzer-core.UndefinedBinaryOperatorResult)
                   : (lhs >> rhs) & static_cast<promoted_type>(~Result{}));
        }

        template<typename Result, typename Lhs>
        CNL_NODISCARD constexpr auto sensible_right_shift(Lhs const& lhs, int rhs)
        -> enable_if_t<(digits<Result>::value > digits<decltype(lhs>>rhs)>::value), Result>
        {
            CNL_ASSERT(rhs>=0);
            using promoted_type = decltype(lhs >> rhs);
            return (rhs>=digits<promoted_type>::value)
                   ? Result{}
                   : static_cast<Result>(lhs >> rhs);
        }

        template<typename Result, typename Lhs>
        CNL_NODISCARD constexpr auto sensible_left_shift(Lhs const& lhs, int rhs)
        -> enable_if_t<digits<Result>::value <= digits<decltype(lhs<<rhs)>::value, Result>
        {
            CNL_ASSERT(rhs>=0);
            using promoted_type = decltype(lhs << rhs);
            using unsigned_type = remove_signedness_t<decltype(lhs & lhs)>;
            return (rhs>=digits<promoted_type>::value)
                   ? Result{}
                   : static_cast<Result>(
                            // TODO: Not reproduced locally. Investigate.
                            // NOLINTNEXTLINE(clang-analyzer-core.UndefinedBinaryOperatorResult)
                           static_cast<unsigned_type>(lhs & sensible_right_shift<Lhs>(~Result{}, rhs)) << rhs);
        }

        template<typename Result, typename Lhs>
        CNL_NODISCARD constexpr auto sensible_left_shift(Lhs const& lhs, int rhs)
        -> enable_if_t<(digits<Result>::value > digits<decltype(lhs<<rhs)>::value), Result>
        {
            return sensible_left_shift<Result>(static_cast<Result>(lhs), rhs);
        }

        template<typename Result, typename Lhs>
        CNL_NODISCARD constexpr auto extra_sensible_right_shift(Lhs const& lhs, int rhs) -> Result
        {
            return (rhs<0)
                    ? sensible_left_shift<Result>(lhs, -rhs)
                    : sensible_right_shift<Result>(lhs, rhs);
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
