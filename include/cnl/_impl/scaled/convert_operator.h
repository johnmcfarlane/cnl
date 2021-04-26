
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_CONVERT_OPERATOR_H)
#define CNL_IMPL_SCALED_CONVERT_OPERATOR_H

#include "../../floating_point.h"
#include "../../fraction.h"
#include "../../integer.h"
#include "../custom_operator/native_tag.h"
#include "../num_traits/fixed_width_scale.h"
#include "../num_traits/scale.h"
#include "../power_value.h"
#include "../scaled_integer/definition.h"
#include "is_scaled_tag.h"

/// compositional numeric library
namespace cnl {
    // integer -> floating
    template<integer Src, scaled_tag SrcScale, floating_point Dest, scaled_tag DestScale>
    requires(_impl::radix_v<SrcScale> == _impl::radix_v<DestScale>) struct custom_operator<
            _impl::convert_op,
            op_value<Src, SrcScale>,
            op_value<Dest, DestScale>> {
        [[nodiscard]] constexpr auto operator()(Src const& from) const
        {
            return Dest(from)
                 * _impl::power_value<Dest, _impl::exponent_v<SrcScale> - _impl::exponent_v<DestScale>, _impl::radix_v<SrcScale>>();
        }
    };

    // floating -> integer
    template<floating_point Input, scaled_tag SrcScale, integer Result, scaled_tag DestScale>
    requires(_impl::radix_v<SrcScale> == _impl::radix_v<DestScale>) struct custom_operator<
            _impl::convert_op,
            op_value<Input, SrcScale>,
            op_value<Result, DestScale>> {
        [[nodiscard]] constexpr auto operator()(Input const& from) const
        {
            return static_cast<Result>(
                    from * _impl::power_value<Input, _impl::exponent_v<SrcScale> - _impl::exponent_v<DestScale>, _impl::radix_v<SrcScale>>());
        }
    };

    // integer -> integer (same Radix)
    template<integer Input, scaled_tag SrcScale, integer Result, scaled_tag DestScale>
    requires(_impl::radix_v<SrcScale> == _impl::radix_v<DestScale>) struct custom_operator<
            _impl::convert_op,
            op_value<Input, SrcScale>, op_value<Result, DestScale>> {
        [[nodiscard]] constexpr auto operator()(Input const& from) const
        {
            // when converting *from* scaled_integer
            return static_cast<Result>(_impl::scale<_impl::exponent_v<SrcScale> - _impl::exponent_v<DestScale>, _impl::radix_v<SrcScale>>(
                    _impl::from_value<Result>(from)));
        }
    };

    // integer -> integer (different Ridixes)
    template<
            integer Input, scaled_tag SrcScale,
            integer Result, scaled_tag DestScale>
    struct custom_operator<
            _impl::convert_op,
            op_value<Input, SrcScale>,
            op_value<Result, DestScale>> {
        [[nodiscard]] constexpr auto operator()(Input const& from) const
        {
            constexpr auto src_exponent{_impl::exponent_v<SrcScale>};
            constexpr auto src_radix{_impl::radix_v<SrcScale>};
            constexpr auto dest_exponent{_impl::exponent_v<DestScale>};
            constexpr auto dest_radix{_impl::radix_v<DestScale>};

            auto result{_impl::from_value<Result>(from)};
            if constexpr (src_exponent > 0) {
                result = _impl::scale<src_exponent, src_radix>(result);
            }
            if constexpr (dest_exponent < 0) {
                result = _impl::scale<-dest_exponent, dest_radix>(result);
            }
            if constexpr (src_exponent < 0) {
                result = _impl::scale<src_exponent, src_radix>(result);
            }
            if constexpr (dest_exponent > 0) {
                result = _impl::scale<-dest_exponent, dest_radix>(result);
            }
            return result;
        }
    };

    // shims between equivalent tags
    template<typename Input, typename Result, scaled_tag DestScale>
    struct custom_operator<_impl::convert_op, op_value<Input>, op_value<Result, DestScale>>
        : custom_operator<
                  _impl::convert_op,
                  op_value<Input, power<0, _impl::radix_v<DestScale>>>,
                  op_value<Result, DestScale>> {
    };

    template<typename Input, scaled_tag SrcScale, typename Result>
    struct custom_operator<_impl::convert_op, op_value<Input, SrcScale>, op_value<Result>>
        : custom_operator<_impl::convert_op, op_value<Input, SrcScale>, op_value<Result, power<0, _impl::radix_v<SrcScale>>>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // conversion from fraction

    namespace _impl {
        // template<integer Rep, scaled_tag Scale>
        // [[nodiscard]] constexpr auto not_scaled_integer(
        //         scaled_integer<Rep, Scale> const& f)
        // {
        //     return _impl::to_rep(f);
        // }

        // template<integer Rep, scaled_tag Scale>
        // inline constexpr auto exponent_v<scaled_integer<Rep, Scale>> = Exponent;

        template<class Quotient, class Dividend, class Divisor>
        struct exponent_shift
            : std::integral_constant<
                      int,
                      _impl::exponent_v<Dividend> - _impl::exponent_v<Divisor> - _impl::exponent_v<Quotient>> {
        };
    }

    template<
            typename SrcNumerator, typename SrcDenominator,
            typename Dest, scaled_tag DestScale>
    struct custom_operator<
            _impl::convert_op,
            op_value<cnl::fraction<SrcNumerator, SrcDenominator>, typename DestScale::identity>,
            op_value<Dest, DestScale>> {
        [[nodiscard]] constexpr auto operator()(
                cnl::fraction<SrcNumerator, SrcDenominator> const& from) const
        {
            static_assert(_impl::exponent_v<Dest> == 0, "TODO");

            return static_cast<Dest>(
                    _impl::fixed_width_scale<
                            _impl::exponent_v<SrcNumerator> - _impl::exponent_v<SrcDenominator> - _impl::exponent_v<DestScale>,
                            _impl::radix_v<DestScale>>(static_cast<Dest>(_impl::not_scaled_integer(from.numerator)))
                    / _impl::not_scaled_integer(from.denominator));
        }
    };
}

#endif  // CNL_IMPL_SCALED_CONVERT_OPERATOR_H
