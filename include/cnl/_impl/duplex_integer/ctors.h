
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_CTORS_H)
#define CNL_IMPL_DUPLEX_INTEGER_CTORS_H

#include "../../floating_point.h"
#include "../../limits.h"
#include "../power_value.h"
#include "operators.h"
#include "type.h"

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // cnl::_impl::calculate_lower
        template<typename Lower, typename Integer>
        requires(digits<Lower> >= digits<Integer>)
                [[nodiscard]] constexpr auto calculate_lower(Integer const& input) -> Lower
        {
            return Lower(input) & numeric_limits<Lower>::max();
        }

        template<typename Lower, typename Integer>
        requires(digits<Lower> < digits<Integer>)
                [[nodiscard]] constexpr auto calculate_lower(Integer const& input) -> Lower
        {
            return static_cast<Lower>(input & static_cast<Integer>(numeric_limits<Lower>::max()));
        }

        // cnl::_impl::calculate upper
        template<typename Upper, typename Lower, typename Integer>
        requires(digits<Lower> >= digits<Integer>)
                [[nodiscard]] constexpr auto calculate_upper(Integer const& input) -> Upper
        {
            // sign-friendly flush
            // TODO: Not reproduced locally. Investigate.
            // NOLINTNEXTLINE(clang-analyzer-core.UndefinedBinaryOperatorResult)
            return static_cast<Upper>((input >> (digits<Integer> - 1)) >> 1);
        }

        template<typename Upper, typename Lower, typename Integer>
        requires(digits<Lower> < digits<Integer>)
                [[nodiscard]] constexpr auto calculate_upper(Integer const& input) -> Upper
        {
            return sensible_right_shift<Upper>(input, digits<Lower>);
        }

        template<typename Upper, typename Lower>
        constexpr duplex_integer<Upper, Lower>::duplex_integer(
                upper_type const& u, lower_type const& l)
            : _upper(u)
            , _lower(l)
        {
        }

        template<typename Upper, typename Lower>
        template<integer Number>
        constexpr duplex_integer<Upper, Lower>::duplex_integer(Number const& n)
            : _upper(calculate_upper<Upper, Lower>(n))
            , _lower(calculate_lower<Lower>(n))
        {
        }

        // Note: floating-point constructor is not a constant expression because it relies on
        // std::fmod and std::fmod is not declared constexpr. (See wg21.link/p0533 for efforts to
        // remedy this.)
        template<typename Upper, typename Lower>
        template<floating_point Number>
        constexpr duplex_integer<Upper, Lower>::duplex_integer(Number const& n)
            : _upper(Upper(n / power_value<Number, lower_width, 2>()))
            , _lower(Lower(std::fmod(n, power_value<Number, lower_width, 2>())))
        {
        }
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_CTORS_H
