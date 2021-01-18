
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
#include "neg_inf_rounding_tag.h"
#include "tie_to_pos_inf_rounding_tag.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<class N>
        struct is_arithmetic_or_integer
            : std::integral_constant<
                      bool, std::is_floating_point<N>::value || numeric_limits<N>::is_integer> {
        };

        template<class N1, class N2>
        struct are_arithmetic_or_integer
            : std::integral_constant<
                      bool,
                      is_arithmetic_or_integer<N1>::value && is_arithmetic_or_integer<N2>::value> {
        };
    }

    /// \cond
    template<typename Source, tag SrcTag, typename Destination>
    requires(!_impl::is_rounding_tag<SrcTag>::value) struct custom_operator<
            _impl::convert_op,
            operand<Source, SrcTag>,
            operand<Destination, native_rounding_tag>> {
        CNL_NODISCARD constexpr Destination operator()(Source const& from) const
        {
            return custom_operator<
                    _impl::convert_op, operand<Source, SrcTag>, operand<Destination>>{}(from);
        }
    };

    template<typename Source, tag SrcTag, typename Destination>
    requires(!_impl::is_rounding_tag<SrcTag>::value && _impl::are_arithmetic_or_integer<Destination, Source>::value) struct custom_operator<_impl::convert_op, operand<Source, SrcTag>, operand<Destination, nearest_rounding_tag>> {
        CNL_NODISCARD constexpr Destination operator()(Source const& from) const
        {
            return numeric_limits<Destination>::is_integer && std::is_floating_point<Source>::value
                         ? static_cast<Destination>(
                                 static_cast<long double>(from) + ((from >= Source{}) ? .5L : -.5L))
                         : static_cast<Destination>(from);
        }
    };

    template<typename Source, tag SrcTag, typename Destination>
    requires(!_impl::is_rounding_tag<SrcTag>::value && _impl::are_arithmetic_or_integer<Destination, Source>::value) struct custom_operator<_impl::convert_op, operand<Source, SrcTag>, operand<Destination, tie_to_pos_inf_rounding_tag>> {
    private:
        CNL_NODISCARD static constexpr Destination floor_residual(Source x, Source x_whole)
        {
            return static_cast<Destination>((x < Source(0)) && (x < x_whole));
        }
        CNL_NODISCARD static constexpr Source floor_int(Source x, Source x_whole)
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
                         ? static_cast<Destination>(floor(from + static_cast<Source>(.5L)))
                         : static_cast<Destination>(from);
        }
    };

    template<typename Source, tag SrcTag, typename Destination>
    requires(!_impl::is_rounding_tag<SrcTag>::value && _impl::are_arithmetic_or_integer<Destination, Source>::value) struct custom_operator<_impl::convert_op, operand<Source, SrcTag>, operand<Destination, neg_inf_rounding_tag>> {
    private:
        CNL_NODISCARD static constexpr Destination floor_residual(Source x, Source x_whole)
        {
            return static_cast<Destination>((x < Source(0)) && (x < x_whole));
        }
        CNL_NODISCARD static constexpr Source floor_int(Source x, Source x_whole)
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
                         ? static_cast<Destination>(floor(from))
                         : static_cast<Destination>(from);
        }
    };
    /// \endcond

    template<typename Source, rounding_tag SrcTag, typename Destination>
    struct custom_operator<_impl::convert_op, operand<Source, SrcTag>, operand<Destination>> {
        CNL_NODISCARD constexpr Destination operator()(Source const& from) const
        {
            return custom_operator<_impl::convert_op, operand<Source>, operand<Destination>>{}(
                    from);
        }
    };

    template<typename Source, rounding_tag SrcRoundingTag, typename Destination, rounding_tag DestRoundingTag>
    struct custom_operator<_impl::convert_op, operand<Source, SrcRoundingTag>, operand<Destination, DestRoundingTag>>
        : custom_operator<_impl::convert_op, operand<Source>, operand<Destination, DestRoundingTag>> {
    };
}

#endif  // CNL_IMPL_ROUNDING_TAGGED_CONVERT_OPERATOR_H
