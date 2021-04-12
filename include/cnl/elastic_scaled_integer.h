
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `cnl::elastic_scaled_integer` type

#if !defined(CNL_ELASTIC_SCALED_INTEGER_H)
#define CNL_ELASTIC_SCALED_INTEGER_H

#include "_impl/charconv/descale.h"
#include "_impl/numbers/adopt_signedness.h"
#include "_impl/scaled/is_scaled_tag.h"
#include "_impl/scaled/power.h"
#include "elastic_integer.h"
#include "numeric_limits.h"
#include "scaled_integer.h"

#include <algorithm>
#include <type_traits>

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // cnl-specific definitions

    /// \brief literal real number approximation that uses fixed-point arithmetic and auto-widens to
    /// avoid overflow
    ///
    /// \tparam Digits the total number of integer and fractional digits stored
    /// \tparam Exponent the exponent by which the number is scale
    /// \tparam Narrowest the most narrow integer type to use to represent values
    ///
    /// \sa elastic_integer

    template<int Digits, scaled_tag Scale = power<>, class Narrowest = signed>
    using elastic_scaled_integer =
            scaled_integer<elastic_integer<Digits, Narrowest>, Scale>;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_elastic_scaled_integer

    /// \brief generate an \ref elastic_scaled_integer object of given value
    ///
    /// \tparam Narrowest the narrowest type to use as storage
    /// in the resultant \ref elastic_scaled_integer object
    /// \tparam Integral the type of Value
    /// \tparam Value the integer number to be represented
    ///
    /// \return the given value to be represented using an \ref elastic_scaled_integer type
    ///
    /// \note The return type is guaranteed to be no larger than is necessary to represent the
    /// value.
    ///
    /// \par Example
    ///
    /// To define an int-sized object using \ref make_elastic_scaled_integer and \ref constant
    /// \snippet snippets.cpp define an int-sized object using make_elastic_scaled_integer and constant

    template<typename Narrowest = int, CNL_IMPL_CONSTANT_VALUE_TYPE Value = 0>
    [[nodiscard]] constexpr auto make_elastic_scaled_integer(constant<Value>)
            -> elastic_scaled_integer<
                    std::max(digits_v<constant<Value>> - trailing_bits(Value), 1),
                    power<trailing_bits(Value)>, Narrowest>
    {
        return Value;
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_elastic_scaled_integer

    ///
    /// \tparam Narrowest the most narrow storage type of the resultant \ref
    /// cnl::elastic_scaled_integer object \tparam Integral the type of value
    ///
    /// \param value the value with which to initialize the elastic_scaled_integer object
    ///
    /// \note The return type is guaranteed to be no larger than is necessary to represent the
    /// value.
    ///
    /// \par Example
    ///
    /// To define a byte-sized object using make_elastic_scaled_integer and _c:
    /// \snippet snippets.cpp define a byte-sized object using \ref make_elastic_scaled_integer and \ref _c
    ///
    /// \brief generate an \ref elastic_scaled_integer object of given value
    template<typename Narrowest = void, typename Integral = int>
    [[nodiscard]] constexpr auto make_elastic_scaled_integer(Integral const& value)
            -> elastic_scaled_integer<
                    numeric_limits<Integral>::digits, power<>,
                    typename std::conditional<
                            std::is_same<void, Narrowest>::value,
                            _impl::adopt_signedness_t<int, Integral>, Narrowest>::type>
    {
        return {value};
    }

    template<typename Narrowest = void, typename Rep = int, scaled_tag Scale = power<>>
    [[nodiscard]] constexpr auto make_elastic_scaled_integer(scaled_integer<Rep, Scale> const& value)
            -> elastic_scaled_integer<
                    numeric_limits<Rep>::digits, Scale,
                    typename std::conditional<
                            std::is_same<void, Narrowest>::value,
                            _impl::adopt_signedness_t<int, Rep>, Narrowest>::type>
    {
        return {value};
    }

    namespace _impl {
        template<integer auto ParsedSignificand, integer auto ParsedExponent, int ParsedRadix, int UdlRadix>
        [[nodiscard]] constexpr auto make_from_udl()
        {
            constexpr auto descaled{
                    descale<decltype(ParsedSignificand), UdlRadix, true>(
                            ParsedSignificand,
                            power<ParsedExponent, ParsedRadix>{})};
            constexpr auto rep{make_elastic_integer(constant<descaled.significand>{})};
            return from_rep<scaled_integer<
                    decltype(rep),
                    power<descaled.exponent, descaled.radix>>>(rep);
        }
    }

    /// \c user-defined literals
    namespace literals {
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::literals::operator "" _cnl

        /// \brief generate an \ref elastic_scaled_integer object using a literal
        ///
        /// \tparam Digits the characters of the literal sequence
        ///
        /// \return the given value to be represented using an \ref elastic_scaled_integer type
        ///
        /// \note The return type is guaranteed to be no larger
        /// than is necessary to represent the maximum value of Integral.
        ///
        /// \par Example
        ///
        /// To define a decimal fixed-point object with value 3.141:
        /// \snippet snippets.cpp define an object using elastic literal

        template<char... Chars>
        [[nodiscard]] constexpr auto operator"" _cnl()
        {
            constexpr auto parsed{_impl::parse_real<intmax, Chars...>()};
            return _impl::make_from_udl<parsed.significand, parsed.exponent, parsed.radix, parsed.radix>();
        }

        /// \brief generate an \ref elastic_scaled_integer object using a binary literal
        ///
        /// \tparam Digits the characters of the literal sequence
        /// \pre Sequence must be representable using binary fractions. E.g. `0.1_cnl2` cannot be
        /// represented using binary fixed-point numbers. This will not compile.
        ///
        /// \return the given value to be represented using an \ref elastic_scaled_integer type
        ///
        /// \note The return type is guaranteed to be no larger
        /// than is necessary to represent the maximum value of Integral.

        template<char... Chars>
        [[nodiscard]] constexpr auto operator"" _cnl2()
        {
            constexpr auto parsed{_impl::parse_real<intmax, Chars...>()};
            return _impl::make_from_udl<parsed.significand, parsed.exponent, parsed.radix, 2>();
        }
    }
}

#endif  // CNL_ELASTIC_SCALED_INTEGER_H
