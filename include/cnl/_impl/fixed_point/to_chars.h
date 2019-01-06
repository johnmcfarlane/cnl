
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_FIXED_POINT_TO_CHARS_H
#define CNL_IMPL_FIXED_POINT_TO_CHARS_H

#include "../../rounding_integer.h"
#include "num_traits.h"
#include "type.h"

#include <iterator>
#include <system_error>

/// compositional numeric library
namespace cnl {
    struct to_chars_result {
        char* ptr;
        std::errc ec;
    };

    namespace _impl {
        template<typename Scalar>
        struct max_decimal_digits;

        template<typename Rep, int Exponent>
        struct max_decimal_digits<fixed_point<Rep, Exponent, 2>> {
            using _scalar = cnl::fixed_point<Rep, Exponent>;
            static constexpr auto _fractional_digits = cnl::_impl::fractional_digits<_scalar>::value;

            static constexpr auto _sign_chars = static_cast<int>(cnl::is_signed<_scalar>::value);
            static constexpr auto _integer_chars = ((cnl::_impl::integer_digits<_scalar>::value + 2) / 3);
            static constexpr auto _radix_chars = static_cast<int>(_fractional_digits > 0);
            static constexpr auto _fractional_chars = max(0, _fractional_digits);

            static constexpr auto value = _sign_chars + _integer_chars + _radix_chars + _fractional_chars;
        };

        template<typename Rep, int Exponent, int Radix>
        constexpr auto trunc(fixed_point<Rep, Radix, Exponent> const& scalar)
        -> decltype(scale<Radix, Exponent>(to_rep(scalar)))
        {
            return scale<Radix, Exponent>(to_rep(scalar));
        }

        template<typename Scalar>
        char itoc(Scalar value) noexcept
        {
            static_assert(
                    std::is_same<
                            typename rounding<Scalar>::type,
                            native_rounding_tag>::value,
                    "wrong rounding type");
            auto c = '0'+static_cast<int>(value);
            return static_cast<char>(c);
        }

        template<class Value>
        char* to_chars_natural(char* const ptr, char const* const last, Value const& value)
        {
            static_assert(
                    fractional_digits<Value>::value==0,
                    "this function won't work with fractional numbers");

            auto const quotient = value/10;

            auto const next_ptr = [&]() {
                if (quotient) {
                    return to_chars_natural(ptr, last, quotient);
                }
                return ptr;
            }();

            if (next_ptr==last || next_ptr==nullptr) {
                return nullptr;
            }

            auto const remainder = value%10;
            *next_ptr = itoc(remainder);

            return next_ptr+1;
        }

        template<typename Rep, int Exponent, int Radix>
        to_chars_result to_chars_fractional(
                char* first,
                char const* const last,
                fixed_point<Rep, Exponent, Radix> value) noexcept
        {
            auto const destination_length = std::distance(static_cast<char const*>(first), last);
            if (destination_length<2) {
                return to_chars_result{first, std::errc{}};
            }

            *first = '.';
            first++;

            do {
                value *= 10;
                auto const unit = trunc(value);
                *first = itoc(unit);
                ++first;

                value -= unit;
                if (!value) {
                    break;
                }
            }
            while (first!=last);

            return to_chars_result{first, std::errc{}};
        }

        template<typename Rep, int Exponent, int Radix>
        to_chars_result to_chars_positive(
                char* const first,
                char* const last,
                fixed_point<Rep, Exponent, Radix> const& value) noexcept
        {
            auto const natural = trunc(value);
            auto const natural_last = to_chars_natural(first, last, natural);
            if (!natural_last) {
                return to_chars_result{last, std::errc::value_too_large};
            }

            auto const fractional = value - natural;
            if (!fractional) {
                return to_chars_result{natural_last, std::errc{}};
            }

            return to_chars_fractional(natural_last, last, fractional);
        }

        template<typename FixedPoint, bool is_signed = is_signed<FixedPoint>::value>
        struct to_chars_non_zero;

        template<typename NativeRoundingRep, int Exponent, int Radix>
        struct to_chars_non_zero<fixed_point<NativeRoundingRep, Exponent, Radix>, false> {
            to_chars_result operator()(
                    char* const first,
                    char* const last,
                    fixed_point<NativeRoundingRep, Exponent, Radix> const& value) const
            {
                // +ve
                return to_chars_positive(first, last, value);
            }
        };

        template<typename NativeRoundingRep, int Exponent, int Radix>
        struct to_chars_non_zero<fixed_point<NativeRoundingRep, Exponent, Radix>, true> {
            to_chars_result operator()(
                    char* const first,
                    char* const last,
                    fixed_point<NativeRoundingRep, Exponent, Radix> const& value) const
            {
                if (value>0.) {
                    // +ve
                    return to_chars_positive(first, last, value);
                }
                else {
                    auto const destination_length = std::distance(first, last);
                    if (destination_length<2) {
                        return to_chars_result{last, std::errc::value_too_large};
                    }

                    // -ve
                    *first = '-';
                    return to_chars_positive(first+1, last, -value);
                }
            }
        };
    }

    template<typename Rep, int Exponent, int Radix>
    to_chars_result to_chars(
            char* const first,
            char* const last,
            cnl::fixed_point<Rep, Exponent, Radix> const& value)
    {
        if (!value) {
            if (first==last) {
                // buffer too small to contain "0"
                return to_chars_result{last, std::errc::value_too_large};
            }

            // zero
            *first = '0';
            return to_chars_result{first+1, std::errc{}};
        }

        using native_rounding_type = typename _impl::set_rounding<decltype(value), _impl::native_rounding_tag>::type;
        auto const native_rounding_value = static_cast<native_rounding_type>(value);

        return _impl::to_chars_non_zero<native_rounding_type>{}(
                first, last, native_rounding_value);
    }
}

#endif  // CNL_IMPL_FIXED_POINT_TO_CHARS_H
