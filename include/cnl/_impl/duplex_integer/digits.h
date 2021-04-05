
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_DIGITS_H)
#define CNL_IMPL_DUPLEX_INTEGER_DIGITS_H

#include "../cnl_assert.h"
#include "../num_traits/digits.h"
#include "declaration.h"
#include "numbers.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // 'sensible' shift operations - for when the compiler is being obtuse

        template<typename Result, typename Lhs>
        [[nodiscard]] constexpr auto sensible_right_shift(Lhs const& lhs, int rhs) -> Result
                requires(digits_v<Result> <= digits_v<decltype(lhs >> rhs)>)
        {
            CNL_ASSERT(rhs >= 0);
            using promoted_type = decltype(lhs >> rhs);
            return static_cast<Result>(
                    (rhs >= digits_v<promoted_type>)
                            ? lhs >> (digits_v<Lhs> - 1) >> 1
                            : (lhs >> rhs) & static_cast<promoted_type>(~Result{}));
        }

        template<typename Result, typename Lhs>
        [[nodiscard]] constexpr auto sensible_right_shift(Lhs const& lhs, int rhs) -> Result
                requires(digits_v<Result> > digits_v<decltype(lhs >> rhs)>)
        {
            CNL_ASSERT(rhs >= 0);
            using promoted_type = decltype(lhs >> rhs);
            return (rhs >= digits_v<promoted_type>) ? Result{}
                                                    : static_cast<Result>(lhs >> rhs);
        }

        template<typename Result, typename Lhs>
        [[nodiscard]] constexpr auto sensible_left_shift(Lhs const& lhs, int rhs) -> Result
                requires(digits_v<Result> <= digits_v<decltype(lhs << rhs)>)
        {
            CNL_ASSERT(rhs >= 0);
            using promoted_type = decltype(lhs << rhs);
            using unsigned_type = numbers::set_signedness_t<promoted_type, false>;
            return (rhs >= digits_v<promoted_type>)
                         ? Result{}
                         : static_cast<Result>(
                                 // TODO: Not reproduced locally. Investigate.
                                 // NOLINTNEXTLINE(clang-analyzer-core.UndefinedBinaryOperatorResult)
                                 static_cast<unsigned_type>(
                                         lhs & sensible_right_shift<Lhs>(~Result{}, rhs))
                                 << rhs);
        }

        template<typename Result, typename Lhs>
        [[nodiscard]] constexpr auto sensible_left_shift(Lhs const& lhs, int rhs) -> Result
                requires(digits_v<Result> > digits_v<decltype(lhs << rhs)>)
        {
            return sensible_left_shift<Result>(static_cast<Result>(lhs), rhs);
        }

        template<typename Result, typename Lhs>
        [[nodiscard]] constexpr auto extra_sensible_right_shift(Lhs const& lhs, int rhs) -> Result
        {
            return (rhs < 0) ? sensible_left_shift<Result>(lhs, -rhs)
                             : sensible_right_shift<Result>(lhs, rhs);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::digits_v<cnl::_impl::duplex_integer<>>

    template<typename Upper, typename Lower>
    inline constexpr int digits_v<_impl::duplex_integer<Upper, Lower>> = digits_v<Upper> + digits_v<Lower>;
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_DIGITS_H
