
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_SQRT_H)
#define CNL_IMPL_SCALED_INTEGER_SQRT_H

#include "../cmath/sqrt.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    /// \overload auto sqrt(scaled_integer<Rep, power<Exponent, Radix>> const& x)
    /// \brief \ref scaled_integer overload of cnl::sqrt
    /// \headerfile cnl/scaled_integer.h
    /// \return square root of `x`
    /// \note This function has O(n) complexity where n is the number of significant digits.
    /// \pre `x` must be non-negative
    /// \pre `Exponent` must be even

    template<typename Rep, int Exponent, int Radix>
    [[nodiscard]] constexpr auto sqrt(scaled_integer<Rep, power<Exponent, Radix>> const& x)
    {
        static_assert(!(Exponent & 1));
        using result_type = scaled_integer<Rep, power<Exponent / 2, Radix>>;
        return _impl::from_rep<result_type>(sqrt(_impl::to_rep(x)));
    }
}

#endif  // CNL_IMPL_SCALED_INTEGER_SQRT_H
