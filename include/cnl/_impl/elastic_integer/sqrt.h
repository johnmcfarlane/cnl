
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ELASTIC_INTEGER_SQRT_H)
#define CNL_IMPL_ELASTIC_INTEGER_SQRT_H

#include "../cmath/sqrt.h"
#include "definition.h"

/// compositional numeric library
namespace cnl {
    /// \overload auto sqrt(elastic_integer<Digits, Narrowest> const& x)
    /// \brief \ref elastic_integer overload of cnl::sqrt
    /// \headerfile cnl/elastic_integer.h
    /// \return square root of `x`
    /// \note This function has O(n) complexity where n is the number of significant digits.
    /// \pre `x` must be non-negative

    template<int Digits, class Narrowest>
    [[nodiscard]] constexpr auto sqrt(elastic_integer<Digits, Narrowest> const& x)
    {
        using result_type = elastic_integer<(Digits + 1) / 2, Narrowest>;
        return _impl::from_rep<result_type>(sqrt(_impl::to_rep(x)));
    }
}

#endif  // CNL_IMPL_ELASTIC_INTEGER_SQRT_H
