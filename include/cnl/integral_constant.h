
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to use of `std::integral_constant` type

#if !defined(CNL_INTEGRAL_CONSTANT_H)
#define CNL_INTEGRAL_CONSTANT_H 1

#include <cnl/cnlint.h>
#include <cnl/numeric.h>
#include <cnl/bits/common.h>

#include <cstdint>

namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_integral_constant type traits

        template<class T>
        struct is_integral_constant : std::false_type {
        };

        template<class Integral, Integral Value>
        struct is_integral_constant<std::integral_constant<Integral, Value>> : std::true_type {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::literals - literal wrapper for std::integral_constant
    //
    // http://codereview.stackexchange.com/a/51576/26421

    namespace literals {
        template<char... Chars>
        constexpr auto operator "" _c()
        -> std::integral_constant<intmax, _cnlint_impl::parse<Chars...,'\0'>()>
        {
            return {};
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// unary operator-(std::integral_constant)

#if ! defined(_MSC_VER) || (_MSC_VER > 1900)
template<class RhsIntegral, RhsIntegral RhsValue>
constexpr std::integral_constant<decltype(-RhsValue), -RhsValue>
operator-(std::integral_constant<RhsIntegral, RhsValue>) noexcept
{
    return std::integral_constant<decltype(-RhsValue), -RhsValue>{};
}
#endif

#endif  // CNL_INTEGRAL_CONSTANT_H
