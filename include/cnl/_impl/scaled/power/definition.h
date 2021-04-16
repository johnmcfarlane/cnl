
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_POWER_DEFINITION_H)
#define CNL_IMPL_SCALED_POWER_DEFINITION_H

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
    /// \sa scaled_integer

    template<int Exponent, int Radix>
    struct power {
        static_assert(Radix >= 2, "Radix must be two or greater");

        ////////////////////////////////////////////////////////////////////////////////
        // constants

        /// value of template parameter, \a Exponent
        constexpr static int exponent = Exponent;

        /// value of template parameter, \a Radix
        constexpr static int radix = Radix;

        ////////////////////////////////////////////////////////////////////////////////
        // types

        using identity = power<0, Radix>;
    };

    /// value of template parameter, \a Exponent

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator+(power<LhsExponent, Radix>, power<RhsExponent, Radix>)
            -> power<std::min(LhsExponent, RhsExponent), Radix>
    {
        return power<std::min(LhsExponent, RhsExponent), Radix>{};
    }

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator-(power<LhsExponent, Radix>, power<RhsExponent, Radix>)
            -> power<std::min(LhsExponent, RhsExponent), Radix>
    {
        return power<std::min(LhsExponent, RhsExponent), Radix>{};
    }

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator*(power<LhsExponent, Radix>, power<RhsExponent, Radix>)
            -> power<LhsExponent + RhsExponent, Radix>
    {
        return power<LhsExponent + RhsExponent, Radix>{};
    }

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator/(power<LhsExponent, Radix>, power<RhsExponent, Radix>)
            -> power<LhsExponent - RhsExponent, Radix>
    {
        return power<LhsExponent - RhsExponent, Radix>{};
    }

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator%(power<LhsExponent, Radix>, power<RhsExponent, Radix>)
            -> power<LhsExponent, Radix>
    {
        return power<LhsExponent, Radix>{};
    }

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator&(power<LhsExponent, Radix>, power<RhsExponent, Radix>)
            -> power<std::min(LhsExponent, RhsExponent), Radix>
    {
        return power<std::min(LhsExponent, RhsExponent), Radix>{};
    }

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator|(power<LhsExponent, Radix>, power<RhsExponent, Radix>)
            -> power<std::min(LhsExponent, RhsExponent), Radix>
    {
        return power<std::min(LhsExponent, RhsExponent), Radix>{};
    }

    template<int LhsExponent, int RhsExponent, int Radix>
    [[nodiscard]] constexpr auto operator^(power<LhsExponent, Radix>, power<RhsExponent, Radix>)
            -> power<std::min(LhsExponent, RhsExponent), Radix>
    {
        return power<std::min(LhsExponent, RhsExponent), Radix>{};
    }
}

#endif  // CNL_IMPL_SCALED_POWER_DEFINITION_H
