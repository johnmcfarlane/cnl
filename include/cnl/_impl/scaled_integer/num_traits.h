
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief \ref cnl::scaled_integer specializations of num_traits traits and similar

#if !defined(CNL_IMPL_SCALED_INTEGER_NUM_TRAITS_H)
#define CNL_IMPL_SCALED_INTEGER_NUM_TRAITS_H 1

#include "type.h"

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // scaled_integer specializations of <num_traits.h> templates

    template<typename Rep, class Scale>
    struct digits<scaled_integer<Rep, Scale>> : digits<Rep> {
    };

    template<typename Rep, class Scale, int MinNumBits>
    struct set_digits<scaled_integer<Rep, Scale>, MinNumBits> {
        using type = scaled_integer<set_digits_t<Rep, MinNumBits>, Scale>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::from_value<cnl::scaled_integer<>>

    template<typename Rep, int Exponent, int Radix, typename Value>
    struct from_value<scaled_integer<Rep, power<Exponent, Radix>>, Value>
            : _impl::from_value_simple<scaled_integer<Value, power<0, Radix>>, Value> {
    };

    template<typename Rep, class Scale, typename ValueRep, class ValueScale>
    struct from_value<scaled_integer<Rep, Scale>, scaled_integer<ValueRep, ValueScale>> : _impl::from_value_simple<
            scaled_integer<from_value_t<Rep, ValueRep>, ValueScale>,
            scaled_integer<ValueRep, ValueScale>> {
    };

    template<typename Rep, int Exponent, int Radix, typename Numerator, typename Denominator>
    struct from_value<scaled_integer<Rep, power<Exponent, Radix>>, fraction<Numerator, Denominator>> {
        CNL_NODISCARD constexpr auto operator()(fraction<Numerator, Denominator> const& value) const
        -> decltype(quotient(value.numerator, value.denominator)) {
            return quotient(value.numerator, value.denominator);
        }
    };

    template<typename Rep, int Exponent, int Radix, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<scaled_integer<Rep, power<Exponent, Radix>>, constant<Value>> : _impl::from_value_simple<
            scaled_integer<
                    set_digits_t<int, _impl::max(digits<int>::value, _impl::used_digits(Value)-trailing_bits(Value))>,
                    power<trailing_bits(Value)>>,
            constant<Value>> {
        // same as deduction guide
    };

    ////////////////////////////////////////////////////////////////////////////////
    // scaled_integer specializations of scaled_integer-specific templates

    namespace _impl {

        // cnl::_impl::fractional_digits
        template <class T>
        struct fractional_digits : std::integral_constant<int, 0> {
        };

        template<typename Rep, int Exponent, int Radix>
        struct fractional_digits<scaled_integer<Rep, power<Exponent, Radix>>>
                : std::integral_constant<int, -Exponent> {
        };

        // cnl::_impl::integer_digits
        template <class T>
        struct integer_digits : std::integral_constant<int, digits<T>::value - fractional_digits<T>::value> {
        };
    }
}

#endif // CNL_IMPL_SCALED_INTEGER_NUM_TRAITS_H
