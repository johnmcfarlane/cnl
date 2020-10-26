
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `cnl::elastic_scaled_integer` type

#if !defined(CNL_ELASTIC_SCALED_INTEGER_H)
#define CNL_ELASTIC_SCALED_INTEGER_H

#include "_impl/num_traits/adopt_signedness.h"
#include "elastic_integer.h"
#include "limits.h"  // NOLINT(modernize-deprecated-headers,  hicpp-deprecated-headers)
#include "scaled_integer.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // cnl-specific definitions

    /// \brief literal real number approximation that uses fixed-point arithmetic and auto-widens to avoid overflow
    ///
    /// \tparam Digits the total number of integer and fractional digits stored
    /// \tparam Exponent the exponent by which the number is scale
    /// \tparam Narrowest the most narrow integer type to use to represent values
    ///
    /// \sa elastic_integer

    template<int Digits, int Exponent = 0, class Narrowest = signed>
    using elastic_scaled_integer = scaled_integer<elastic_integer<Digits, Narrowest>, power<Exponent>>;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_elastic_scaled_integer

    /// \brief generate an \ref cnl::elastic_scaled_integer object of given value
    ///
    /// \tparam Narrowest the narrowest type to use as storage
    /// in the resultant \ref cnl::elastic_scaled_integer object
    /// \tparam Integral the type of Value
    /// \tparam Value the integer number to be represented
    ///
    /// \return the given value to be represented using an \ref cnl::elastic_scaled_integer type
    ///
    /// \note The return type is guaranteed to be no larger than is necessary to represent the value.
    ///
    /// \par Example
    ///
    /// To define an int-sized object using \ref make_elastic_scaled_integer and \ref cnl::constant
    /// \snippet snippets.cpp define an int-sized object using make_elastic_scaled_integer and constant

    template<
            typename Narrowest = int,
            CNL_IMPL_CONSTANT_VALUE_TYPE Value = 0>
    CNL_NODISCARD constexpr auto
    make_elastic_scaled_integer(constant<Value>)
    -> elastic_scaled_integer<
            _impl::max(digits<constant<Value>>::value-trailing_bits(Value), 1),
            trailing_bits(Value),
            Narrowest>
    {
        return Value;
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_elastic_scaled_integer

    ///
    /// \tparam Narrowest the most narrow storage type of the resultant \ref cnl::elastic_scaled_integer object
    /// \tparam Integral the type of value
    ///
    /// \param value the value with which to initialize the elastic_scaled_integer object
    ///
    /// \note The return type is guaranteed to be no larger than is necessary to represent the value.
    ///
    /// \par Example
    ///
    /// To define a byte-sized object using make_elastic_scaled_integer and _c:
    /// \snippet snippets.cpp define a byte-sized object using \ref make_elastic_scaled_integer and \ref _c
    ///
    /// \brief generate an \ref cnl::elastic_scaled_integer object of given value
    template<typename Narrowest = void, typename Integral = int>
    CNL_NODISCARD constexpr auto
    make_elastic_scaled_integer(Integral const& value)
    -> elastic_scaled_integer<
            numeric_limits<Integral>::digits,
            0,
            typename std::conditional<
                    std::is_same<void, Narrowest>::value,
                    _impl::adopt_signedness_t<int, Integral>,
                    Narrowest>::type>
    {
        return {value};
    }

    template<typename Narrowest = void, typename Rep = int, int Exponent = 0, int Radix = 2>
    CNL_NODISCARD constexpr auto
    make_elastic_scaled_integer(scaled_integer<Rep, power<Exponent, Radix>> const& value)
    -> elastic_scaled_integer<
            numeric_limits<Rep>::digits,
            Exponent,
            typename std::conditional<
                    std::is_same<void, Narrowest>::value,
                    _impl::adopt_signedness_t<int, Rep>,
                    Narrowest>::type>
    {
        return {value};
    }

    /// \c user-defined literals
    namespace literals {
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::literals::operator "" _elastic

        /// \brief generate an \ref cnl::elastic_scaled_integer object using a literal
        ///
        /// \tparam Digits the characters of the literal sequence
        ///
        /// \return the given value to be represented using an \ref cnl::elastic_scaled_integer type
        ///
        /// \note The return type is guaranteed to be no larger
        /// than is necessary to represent the maximum value of Integral.
        ///
        /// \par Example
        ///
        /// To define an int-sized object with value 1536:
        /// \snippet snippets.cpp define an object using elastic literal

        template<char... Chars>
        CNL_NODISCARD constexpr auto operator "" _elastic()
        {
            return make_elastic_scaled_integer<int>(
                    constant<_cnlint_impl::parse<sizeof...(Chars)+1>({Chars..., '\0'})>{});
        }
    }
}

#endif  // CNL_ELASTIC_SCALED_INTEGER_H
