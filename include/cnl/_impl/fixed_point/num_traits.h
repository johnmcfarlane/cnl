
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief \ref cnl::fixed_point specializations of num_traits traits and similar

#if !defined(CNL_FIXED_POINT_NUM_TRAITS_H)
#define CNL_FIXED_POINT_NUM_TRAITS_H 1

#include "type.h"

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // fixed_point specializations of <num_traits.h> templates

    template<typename Rep, int Exponent, int Radix>
    struct digits<fixed_point<Rep, Exponent, Radix>> : digits<Rep> {
    };

    template<typename Rep, int Exponent, int Radix, int MinNumBits>
    struct set_digits<fixed_point<Rep, Exponent, Radix>, MinNumBits> {
        using type = fixed_point<set_digits_t<Rep, MinNumBits>, Exponent, Radix>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::from_value<cnl::fixed_point<>>

    template<typename Rep, int Exponent, int Radix, typename Value>
    struct from_value<fixed_point<Rep, Exponent, Radix>, Value>
            : _impl::from_value_simple<fixed_point<Value, 0, Radix>, Value> {
    };

    template<typename Rep, int Exponent, int Radix, typename ValueRep, int ValueExponent>
    struct from_value<fixed_point<Rep, Exponent, Radix>, fixed_point<ValueRep, ValueExponent>> : _impl::from_value_simple<
            fixed_point<from_value_t<Rep, ValueRep>, ValueExponent>,
            fixed_point<ValueRep, ValueExponent>> {
    };

    template<typename Rep, int Exponent, int Radix, typename Numerator, typename Denominator>
    struct from_value<fixed_point<Rep, Exponent, Radix>, fraction<Numerator, Denominator>> {
        constexpr auto operator()(fraction<Numerator, Denominator> const& value) const
        -> decltype(quotient(value.numerator, value.denominator)) {
            return quotient(value.numerator, value.denominator);
        }
    };

    template<typename Rep, int Exponent, int Radix, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<fixed_point<Rep, Exponent, Radix>, constant<Value>> : _impl::from_value_simple<
            fixed_point<
                    set_digits_t<int, _impl::max(digits<int>::value, _impl::used_digits(Value)-trailing_bits(Value))>,
                    trailing_bits(Value)>,
            constant<Value>> {
        // same as deduction guide
    };

    ////////////////////////////////////////////////////////////////////////////////
    // fixed_point specializations of fixed_point-specific templates

    namespace _impl {

        // cnl::_impl::fractional_digits
        template <class T>
        struct fractional_digits : std::integral_constant<int, 0> {
        };

        template<typename Rep, int Exponent, int Radix>
        struct fractional_digits<fixed_point<Rep, Exponent, Radix>> : std::integral_constant<int, -Exponent> {
        };

        // cnl::_impl::integer_digits
        template <class T>
        struct integer_digits : std::integral_constant<int, digits<T>::value - fractional_digits<T>::value> {
        };
    }
}

#endif // CNL_FIXED_POINT_NUM_TRAITS_H
