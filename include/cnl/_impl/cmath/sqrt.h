
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_CMATH_SQRT_H)
#define CNL_IMPL_CMATH_SQRT_H

#include "../../integer.h"
#include "../cnl_assert.h"
#include "../num_traits/digits.h"

/// compositional numeric library
namespace cnl {
    /// \brief integer overload of cnl::sqrt
    /// \headerfile cnl/cmath.h
    /// \return square root of `x`
    /// \note This function has O(n) complexity where n is the number of significant digits.
    /// \pre `x` must be non-negative

    template<integer Integer>
    [[nodiscard]] constexpr auto sqrt(Integer const& x)
    {
        // https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Binary_numeral_system_.28base_2.29
        CNL_ASSERT(x >= Integer{0});

        auto root = +Integer{0};
        auto bit = Integer{1} << ((digits_v<Integer> - 1) & ~1);
        auto num = decltype(root + bit){x};

        while (bit > num) {
            bit >>= 2;
        }

        while (bit) {
            if (num >= root + bit) {
                num -= root + bit;
                root = (root >> 1) + bit;
            } else {
                root >>= 1;
            }
            bit >>= 2;
        }
        return root;
    }
}

#endif  // CNL_IMPL_CMATH_SQRT_H
