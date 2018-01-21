
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief \ref cnl::fixed_point specializations of num_traits traits and similar

#if !defined(CNL_FIXED_POINT_NUM_TRAITS_H)
#define CNL_FIXED_POINT_NUM_TRAITS_H 1

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // fixed_point specializations of number_base-specific templates

    namespace _impl {
        // cnl::_impl::get_rep<fixed_point<>>
        template <class Rep, int Exponent>
        struct get_rep<fixed_point<Rep, Exponent>> {
            using type = Rep;
        };

        // cnl::_impl::set_rep<fixed_point<>>
        template <class OldRep, int Exponent, class NewRep>
        struct set_rep<fixed_point<OldRep, Exponent>, NewRep> {
            using type = fixed_point<NewRep, Exponent>;
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // fixed_point specializations of <num_traits.h> templates

    template <class Rep, int Exponent>
    struct digits<fixed_point<Rep, Exponent>> : digits<Rep> {
    };

    template <class Rep, int Exponent, _digits_type MinNumBits>
    struct set_digits<fixed_point<Rep, Exponent>, MinNumBits> {
        using type = fixed_point<set_digits_t<Rep, MinNumBits>, Exponent>;
    };

    template <class Rep, int Exponent, class Value>
    struct from_value<fixed_point<Rep, Exponent>, Value> {
        using type = fixed_point<Value>;
    };

    template<class Rep, int Exponent, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<fixed_point<Rep, Exponent>, constant<Value>> {
        // same as deduction guide
        using type = fixed_point<
                set_digits_t<int, _impl::max(digits<int>::value, used_bits(Value)-trailing_bits(Value))>,
                trailing_bits(Value)>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // fixed_point specializations of fixed_point-specific templates

    namespace _impl {

        // cnl::_impl::fractional_digits
        template <class T>
        struct fractional_digits : std::integral_constant<_digits_type, 0> {
        };

        template <class Rep, int Exponent>
        struct fractional_digits<fixed_point<Rep, Exponent>> : std::integral_constant<_digits_type, -Exponent> {
        };

        // cnl::_impl::integer_digits
        template <class T>
        struct integer_digits : std::integral_constant<_digits_type, digits<T>::value - fractional_digits<T>::value> {
        };
    }
}

#endif // CNL_FIXED_POINT_NUM_TRAITS_H
