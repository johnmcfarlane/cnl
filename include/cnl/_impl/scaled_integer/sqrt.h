
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_SQRT_H)
#define CNL_IMPL_SCALED_INTEGER_SQRT_H

#include "../cmath/sqrt.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<scaled_tag Scale>
        struct sqrt_result_scale;

        template<int Exponent, int Radix>
        struct sqrt_result_scale<power<Exponent, Radix>>
            : std::type_identity<power<Exponent / 2, Radix>> {
        };

        template<int Exponent, int Radix>
        struct sqrt_result_scale<quasi_exact<Exponent, Radix>>
            : std::type_identity<quasi_exact<Exponent / 2, Radix>> {
        };

        template<scaled_tag Scale>
        using sqrt_result_scale_t = typename sqrt_result_scale<Scale>::type;
    }

    /// \overload auto sqrt(scaled_integer<Rep, Scale> const& x)
    /// \brief \ref scaled_integer overload of cnl::sqrt
    /// \headerfile cnl/scaled_integer.h
    /// \return square root of `x`
    /// \note This function has O(n) complexity where n is the number of significant digits.
    /// \pre `x` must be non-negative
    /// \pre `Exponent` must be even

    template<typename Rep, scaled_tag Scale>
    [[nodiscard]] constexpr auto sqrt(scaled_integer<Rep, Scale> const& x)
    {
        static_assert(!(_impl::exponent_v<Scale> & 1));
        using result_type = scaled_integer<Rep, _impl::sqrt_result_scale_t<Scale>>;
        return _impl::from_rep<result_type>(sqrt(_impl::to_rep(x)));
    }
}

#endif  // CNL_IMPL_SCALED_INTEGER_SQRT_H
