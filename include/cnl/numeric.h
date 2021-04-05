
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file cnl/numeric.h
/// \brief functions that might belong in the \verbatim<numeric>\endverbatim header

#if !defined(CNL_NUMERIC_H)
#define CNL_NUMERIC_H

#include "bit.h"

#include "_impl/num_traits/unwrap.h"
#include "_impl/used_digits.h"

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::trailing_bits

    namespace _numeric_impl {
        template<class Integer, bool IsSigned>
        struct trailing_bits {
            [[nodiscard]] constexpr auto operator()(Integer const& n) const noexcept
            {
                return countr_zero(n);
            }
        };

        template<class Integer>
        struct trailing_bits<Integer, true> {
            [[nodiscard]] constexpr auto operator()(Integer const& n) const noexcept
            {
                using unsigned_type = numbers::set_signedness_t<Integer, false>;
                return countr_zero(static_cast<unsigned_type>(n));
            }
        };
    }

    // count of the right redundant trailing bits
    template<class Integer>
    [[nodiscard]] constexpr auto trailing_bits(Integer const& value)
    {
        return value ? _numeric_impl::trailing_bits<Integer, numbers::signedness_v<Integer>>()(value)
                     : 0;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::used_digits

    template<typename Integer>
    [[nodiscard]] constexpr auto used_digits(
            Integer const& value, int radix = numeric_limits<Integer>::radix)
    {
        return _impl::used_digits_signed<numbers::signedness_v<Integer>>{}(unwrap(value), radix);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::leading_bits

    template<class Integer>
    [[nodiscard]] constexpr auto leading_bits(Integer const& value)
    {
        return digits_v<Integer> - cnl::used_digits(value);
    }
}

#endif  // CNL_NUMERIC_H
