
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_SCALED_INTEGER_POWER_H
#define CNL_IMPL_SCALED_INTEGER_POWER_H

#include "power_value.h"
#include "num_traits/scale.h"
#include "operators/is_same_tag_family.h"
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

    namespace _impl {
        template<int Exponent1, int Radix1, int Exponent2, int Radix2>
        struct is_same_tag_family<power<Exponent1, Radix1>, power<Exponent2, Radix2>> : std::true_type {
        };
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
            return static_cast<Result>(_impl::scale<SrcExponent-DestExponent, Radix>(_impl::from_value<Result>(from)));
        }
    };

    // shims between equivalent tags
    template<
            int DestExponent, int DestRadix,
            typename Result, typename Input>
    struct convert_operator<
            power<DestExponent, DestRadix>,
            _impl::native_tag,
            Result, Input>
            : convert_operator<
                    power<DestExponent, DestRadix>,
                    power<0, DestRadix>,
                    Result, Input>{
    };

    template<
            int SrcExponent, int SrcRadix,
            typename Result, typename Input>
    struct convert_operator<
            _impl::native_tag,
            power<SrcExponent, SrcRadix>,
            Result, Input>
            : convert_operator<
                    power<0, SrcRadix>,
                    power<SrcExponent, SrcRadix>,
                    Result, Input>{
    };
}

#endif //CNL_IMPL_SCALED_INTEGER_POWER_H
