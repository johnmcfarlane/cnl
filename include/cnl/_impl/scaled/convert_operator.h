
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_SCALED_CONVERT_OPERATOR_H
#define CNL_IMPL_SCALED_CONVERT_OPERATOR_H

#include "power.h"
#include "../num_traits/scale.h"
#include "../operators/native_tag.h"
#include "../power_value.h"

/// compositional numeric library
namespace cnl {
    // integer -> floating
    template<
            int Exponent, int Radix,
            typename Result, typename Input>
    struct convert_operator<
            power<Exponent, Radix>,
            Result, Input,
            _impl::enable_if_t<cnl::numeric_limits<Result>::is_iec559 && cnl::numeric_limits<Input>::is_integer>> {
        static_assert(cnl::numeric_limits<Input>::is_integer, "");

        CNL_NODISCARD constexpr Result operator()(Input const& from) const
        {
            return Result(from)*_impl::power_value<Result, Exponent, Radix>();
        }
    };

    // floating -> integer
    template<
            int Exponent, int Radix,
            typename Result, typename Input>
    struct convert_operator<
            power<Exponent, Radix>,
            Result, Input,
            _impl::enable_if_t<cnl::numeric_limits<Result>::is_integer && cnl::numeric_limits<Input>::is_iec559>> {
        CNL_NODISCARD constexpr Result operator()(Input const& from) const
        {
            return static_cast<Result>(from*_impl::power_value<Input, Exponent, Radix>());
        }
    };

    // integer -> integer
    template<
            int Exponent, int Radix,
            typename Result, typename Input>
    struct convert_operator<
            power<Exponent, Radix>,
            Result, Input,
            _impl::enable_if_t<cnl::numeric_limits<Result>::is_integer && cnl::numeric_limits<Input>::is_integer>> {
        CNL_NODISCARD constexpr Result operator()(Input const& from) const
        {
            // when converting *from* scaled_integer
            return static_cast<Result>(_impl::scale<Exponent, Radix>(from));
        }
    };
}

#endif //CNL_IMPL_SCALED_CONVERT_OPERATOR_H
