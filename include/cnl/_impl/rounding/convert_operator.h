
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_TAGGED_CONVERT_OPERATOR_H)
#define CNL_IMPL_ROUNDING_TAGGED_CONVERT_OPERATOR_H

#include "../../limits.h"
#include "../operators/native_tag.h"
#include "../type_traits/enable_if.h"
#include "native_rounding_tag.h"
#include "nearest_rounding_tag.h"
#include "tie_to_pos_inf_rounding_tag.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<class N>
        struct is_arithmetic_or_integer
                : std::integral_constant<bool, numeric_limits<N>::is_specialized> {
        };

        template<class N1, class N2>
        struct are_arithmetic_or_integer
                : std::integral_constant<bool,
                        is_arithmetic_or_integer<N1>::value && is_arithmetic_or_integer<N2>::value> {
        };
    }

    template<class SrcTag, typename Destination, typename Source>
    struct convert_operator<
            native_rounding_tag, SrcTag, Destination, Source,
            _impl::enable_if_t<!_impl::is_rounding_tag<SrcTag>::value>> {
        CNL_NODISCARD constexpr Destination operator()(Source const& from) const
        {
            return convert_operator<_impl::native_tag, SrcTag, Destination, Source>{}(from);
        }
    };

    template<class SrcTag, typename Destination, typename Source>
    struct convert_operator<
            nearest_rounding_tag, SrcTag, Destination, Source,
            _impl::enable_if_t<
                    !_impl::is_rounding_tag<SrcTag>::value
                    &&_impl::are_arithmetic_or_integer<Destination, Source>::value>> {
        CNL_NODISCARD constexpr Destination operator()(Source const& from) const
        {
            return numeric_limits<Destination>::is_integer && std::is_floating_point<Source>::value
                    ? static_cast<Destination>(static_cast<long double>(from)+((from >= Source{}) ? .5L : -.5L))
                    : static_cast<Destination>(from);
        }
    };

    template<class SrcTag, typename Destination, typename Source>
    struct convert_operator<
            tie_to_pos_inf_rounding_tag, SrcTag, Destination, Source,
            _impl::enable_if_t<_impl::are_arithmetic_or_integer<Destination, Source>::value>> {
    private:
        CNL_NODISCARD static constexpr Source ceil(Source x)
        {
            return static_cast<Source>((x - static_cast<Source>(static_cast<Destination>(x))) > 0
                ? static_cast<Destination>(x+1)
                : static_cast<Destination>(x));
        }
        CNL_NODISCARD static constexpr Destination floor_residual(Source x, Source x_whole)
        {
            return static_cast<Destination>((x < Source(0)) && (x < x_whole));
        }
        CNL_NODISCARD static constexpr Source floor_int (Source x, Source x_whole)
        {
            return (x_whole - static_cast<Source>(floor_residual(x, x_whole)));
        }
        CNL_NODISCARD static constexpr Source floor(Source x)
        {
            return floor_int(x, static_cast<Source>(static_cast<Destination>(x)));
        }
    public:
        CNL_NODISCARD constexpr Destination operator()(Source const& from) const
        {
            return numeric_limits<Destination>::is_integer && std::is_floating_point<Source>::value
                    ? static_cast<Destination>(ceil(floor(2*from)/2))
                    : static_cast<Destination>(from);
        }
    };

    template<class SrcTag, typename Destination, typename Source>
    struct convert_operator<_impl::native_tag, SrcTag, Destination, Source,
            _impl::enable_if_t<_impl::is_rounding_tag<SrcTag>::value>> {
        CNL_NODISCARD constexpr Destination operator()(Source const& from) const
        {
            return convert_operator<_impl::native_tag, _impl::native_tag, Destination, Source>{}(from);
        }
    };

    template<class DestRoundingTag, typename Destination, class SrcRoundingTag, typename Source>
    struct convert_operator<
            DestRoundingTag, SrcRoundingTag,
            Destination, Source,
            _impl::enable_if_t<
                    _impl::is_rounding_tag<DestRoundingTag>::value&&_impl::is_rounding_tag<SrcRoundingTag>::value>>
            : convert_operator<DestRoundingTag, _impl::native_tag, Destination, Source> {
    };
}

#endif  // CNL_IMPL_ROUNDING_TAGGED_CONVERT_OPERATOR_H
