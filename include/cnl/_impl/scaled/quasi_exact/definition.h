
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_QUASI_EXACT_DEFINITION_H)
#define CNL_IMPL_SCALED_QUASI_EXACT_DEFINITION_H

#include "declaration.h"

#include <algorithm>

/// compositional numeric library
namespace cnl {
    /// \brief tag representing the scaling of an integer by a fixed factor
    /// \headerfile cnl/scaled_integer.h
    ///
    /// \tparam Exponent the exponent used to scale the integer value; defaults to 0
    /// \tparam Radix the base used to scale the integer value; defaults to 2
    ///
    /// \note This template is identical to \ref power with the exception that it provides an
    /// alternative divide operation, described as 'quasi-exact' in [P1368](http://wg21.link/p1368r1).
    ///
    /// \sa scaled_integer, power

    template<int Exponent, int Radix>
    struct quasi_exact {
        static_assert(Radix >= 2, "Radix must be two or greater");

        ////////////////////////////////////////////////////////////////////////////////
        // constants

        /// value of template parameter, \a Exponent
        constexpr static int exponent = Exponent;

        /// value of template parameter, \a Radix
        constexpr static int radix = Radix;

        ////////////////////////////////////////////////////////////////////////////////
        // types

        using identity = quasi_exact<0, Radix>;
    };

    /// value of template parameter, \a Exponent

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator+(quasi_exact<LhsExponent, Radix>, quasi_exact<RhsExponent, Radix>)
            -> quasi_exact<std::min(LhsExponent, RhsExponent), Radix>
    {
        return quasi_exact<std::min(LhsExponent, RhsExponent), Radix>{};
    }

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator-(quasi_exact<LhsExponent, Radix>, quasi_exact<RhsExponent, Radix>)
            -> quasi_exact<std::min(LhsExponent, RhsExponent), Radix>
    {
        return quasi_exact<std::min(LhsExponent, RhsExponent), Radix>{};
    }

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator*(quasi_exact<LhsExponent, Radix>, quasi_exact<RhsExponent, Radix>)
            -> quasi_exact<LhsExponent + RhsExponent, Radix>
    {
        return quasi_exact<LhsExponent + RhsExponent, Radix>{};
    }

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator/(quasi_exact<LhsExponent, Radix>, quasi_exact<RhsExponent, Radix>)
            -> quasi_exact<LhsExponent - RhsExponent, Radix>
    {
        return quasi_exact<LhsExponent - RhsExponent, Radix>{};
    }

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator%(quasi_exact<LhsExponent, Radix>, quasi_exact<RhsExponent, Radix>)
            -> quasi_exact<LhsExponent, Radix>
    {
        return quasi_exact<LhsExponent, Radix>{};
    }

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator&(quasi_exact<LhsExponent, Radix>, quasi_exact<RhsExponent, Radix>)
            -> quasi_exact<std::min(LhsExponent, RhsExponent), Radix>
    {
        return quasi_exact<std::min(LhsExponent, RhsExponent), Radix>{};
    }

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator|(quasi_exact<LhsExponent, Radix>, quasi_exact<RhsExponent, Radix>)
            -> quasi_exact<std::min(LhsExponent, RhsExponent), Radix>
    {
        return quasi_exact<std::min(LhsExponent, RhsExponent), Radix>{};
    }

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator^(quasi_exact<LhsExponent, Radix>, quasi_exact<RhsExponent, Radix>)
            -> quasi_exact<std::min(LhsExponent, RhsExponent), Radix>
    {
        return quasi_exact<std::min(LhsExponent, RhsExponent), Radix>{};
    }
}

#endif  // CNL_IMPL_SCALED_QUASI_EXACT_DEFINITION_H
