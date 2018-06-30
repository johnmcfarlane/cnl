
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
        template<typename Rep, int Exponent, int Radix>
        struct get_rep<fixed_point<Rep, Exponent, Radix>> {
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

    template<typename Rep, int Exponent, int Radix>
    struct digits<fixed_point<Rep, Exponent, Radix>> : digits<Rep> {
    };

    template<typename Rep, int Exponent, int Radix, _digits_type MinNumBits>
    struct set_digits<fixed_point<Rep, Exponent, Radix>, MinNumBits> {
        using type = fixed_point<set_digits_t<Rep, MinNumBits>, Exponent, Radix>;
    };

    /// \brief \ref fixed_point specialization of \ref from_rep
    /// \headerfile cnl/fixed_point.h
    ///
    /// \tparam Exponent the \c Exponent parameter of the generated \ref fixed_point type
    /// \tparam ArchetypeRep ignored; replaced by \c Rep
    template<typename ArchetypeRep, int Exponent, int Radix>
    struct from_rep<fixed_point<ArchetypeRep, Exponent, Radix>> {
        /// \brief generates a \ref fixed_point equivalent to \c r in type and value
        template<typename Rep>
        constexpr auto operator()(Rep const& r) const
        -> fixed_point<Rep, Exponent, Radix>
        {
            return fixed_point<Rep, Exponent, Radix>(r, 0);
        }
    };

    /// \brief \ref fixed_point overload of \ref to_rep(Number const& number)
    /// \headerfile cnl/fixed_point.h
    template<typename Rep, int Exponent, int Radix>
    constexpr Rep to_rep(fixed_point<Rep, Exponent, Radix> const& number)
    {
        using base_type = typename fixed_point<Rep, Exponent, Radix>::_base;
        return to_rep(static_cast<base_type const&>(number));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::from_value<cnl::fixed_point<>>

    template<typename Rep, int Exponent, int Radix, typename Value>
    struct from_value<fixed_point<Rep, Exponent, Radix>, Value>
            : _impl::from_value_simple<Value, fixed_point<Value, 0, Radix>> {
    };

    template<typename Rep, int Exponent, int Radix, typename ValueRep, int ValueExponent>
    struct from_value<fixed_point<Rep, Exponent, Radix>, fixed_point<ValueRep, ValueExponent>> : _impl::from_value_simple<
            fixed_point<ValueRep, ValueExponent>, fixed_point<from_value_t<Rep, ValueRep>, ValueExponent>> {
    };

    template<typename Rep, int Exponent, int Radix, typename Numerator, typename Denominator>
    struct from_value<fixed_point<Rep, Exponent, Radix>, fractional<Numerator, Denominator>> {
        constexpr auto operator()(fractional<Numerator, Denominator> const& value) const
        -> decltype(quotient(value.numerator, value.denominator)) {
            return quotient(value.numerator, value.denominator);
        }
    };

    template<typename Rep, int Exponent, int Radix, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    struct from_value<fixed_point<Rep, Exponent, Radix>, constant<Value>> : _impl::from_value_simple<constant<Value>,
            fixed_point<
                    set_digits_t<int, _impl::max(digits<int>::value, _impl::used_digits(Value)-trailing_bits(Value))>,
                    trailing_bits(Value)>> {
        // same as deduction guide
    };

    ////////////////////////////////////////////////////////////////////////////////
    // fixed_point specializations of fixed_point-specific templates

    namespace _impl {

        // cnl::_impl::fractional_digits
        template <class T>
        struct fractional_digits : std::integral_constant<_digits_type, 0> {
        };

        template<typename Rep, int Exponent, int Radix>
        struct fractional_digits<fixed_point<Rep, Exponent, Radix>> : std::integral_constant<_digits_type, -Exponent> {
        };

        // cnl::_impl::integer_digits
        template <class T>
        struct integer_digits : std::integral_constant<_digits_type, digits<T>::value - fractional_digits<T>::value> {
        };
    }
}

#endif // CNL_FIXED_POINT_NUM_TRAITS_H
