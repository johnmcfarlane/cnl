
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_SCALED_INTEGER_POWER_H
#define CNL_IMPL_SCALED_INTEGER_POWER_H

#include "power_value.h"
#include "num_traits/scale.h"
#include "operators/native_tag.h"

/// compositional numeric library
namespace cnl {
    template<int Exponent = 0, int Radix = 2>
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
    template<int Exponent, int Radix>
    constexpr int power<Exponent, Radix>::exponent;

    template<int LhsExponent, int RhsExponent, int Radix>
    constexpr auto operator/(power<LhsExponent, Radix>, power<RhsExponent, Radix>)
    -> power<LhsExponent-RhsExponent, Radix>
    {
        return power<LhsExponent-RhsExponent, Radix>{};
    }

    // integer -> floating
    template<
            int DestExponent, int SrcExponent, int Radix,
            typename Dest, typename Src>
    struct convert_operator<
            power<DestExponent, Radix>,
            power<SrcExponent, Radix>,
            Dest, Src,
            _impl::enable_if_t<cnl::numeric_limits<Dest>::is_iec559 && cnl::numeric_limits<Src>::is_integer>> {
        static_assert(cnl::numeric_limits<Src>::is_integer, "");

        CNL_NODISCARD constexpr Dest operator()(Src const& from) const
        {
            return Dest(from)*_impl::power_value<Dest, SrcExponent-DestExponent, Radix>();
        }
    };

    // floating -> integer
    template<
            int DestExponent, int SrcExponent, int Radix,
            typename Result, typename Input>
    struct convert_operator<
            power<DestExponent, Radix>,
            power<SrcExponent, Radix>,
            Result, Input,
            _impl::enable_if_t<cnl::numeric_limits<Result>::is_integer && cnl::numeric_limits<Input>::is_iec559>> {
        CNL_NODISCARD constexpr Result operator()(Input const& from) const
        {
            return static_cast<Result>(from*_impl::power_value<Input, SrcExponent-DestExponent, Radix>());
        }
    };

    // integer -> integer
    template<
            int DestExponent, int SrcExponent, int Radix,
            typename Result, typename Input>
    struct convert_operator<
            power<DestExponent, Radix>,
            power<SrcExponent, Radix>,
            Result, Input,
            _impl::enable_if_t<cnl::numeric_limits<Result>::is_integer && cnl::numeric_limits<Input>::is_integer>> {
        CNL_NODISCARD constexpr Result operator()(Input const& from) const
        {
            // when converting *from* scaled_integer
            return static_cast<Result>(_impl::scale<SrcExponent-DestExponent, Radix>(from));
        }
    };
}

#endif //CNL_IMPL_SCALED_INTEGER_POWER_H
