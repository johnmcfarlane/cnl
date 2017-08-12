
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `cnl::elastic_fixed_point` type

#if !defined(CNL_ELASTIC_FIXED_POINT_H)
#define CNL_ELASTIC_FIXED_POINT_H 1

#include "cnl/elastic_integer.h"
#include <cnl/fixed_point.h>

/// compositional numeric library
namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // cnl-specific definitions

    /// \brief literal real number approximation that uses fixed-point arithmetic and auto-widens to avoid overflow
    ///
    /// \tparam IntegerDigits the number of integer digits that can be stored
    /// \tparam FractionalDigits the number of fractional digits that can be stored
    /// \tparam Narrowest the most narrow integer type to use to represent values
    ///
    /// \sa elastic_integer

    template<int Digits, int Exponent = 0, class Narrowest = signed>
    using elastic_fixed_point = fixed_point<elastic_integer<Digits, Narrowest>, Exponent>;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_elastic_fixed_point

    /// \brief generate an \ref cnl::elastic_fixed_point object of given value
    ///
    /// \tparam Narrowest the narrowest type to use as storage
    /// in the resultant \ref cnl::elastic_fixed_point object
    /// \tparam Integral the type of Value
    /// \tparam Value the integer number to be represented
    ///
    /// \return the given value to be represented using an \ref cnl::elastic_fixed_point type
    ///
    /// \note The return type is guaranteed to be no larger than is necessary to represent the value.
    ///
    /// \par Example
    ///
    /// To define an int-sized object using \ref make_elastic_fixed_point and \ref const_integer:
    /// \snippet snippets.cpp define an int-sized object using make_elastic_fixed_point and const_integer

    template<
            typename Narrowest = int,
            typename Integral = int,
            Integral Value = 0>
    constexpr elastic_fixed_point<_impl::max(_impl::used_bits_symmetric(Value)-trailing_bits(Value), 1), trailing_bits(Value), Narrowest>
    make_elastic_fixed_point(const_integer<Integral, Value> = const_integer<Integral, Value>{})
    {
        return Value;
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::make_elastic_fixed_point

    ///
    /// \tparam Narrowest the most narrow storage type of the resultant \ref cnl::elastic_fixed_point object
    /// \tparam Integral the type of \ref value
    ///
    /// \note The return type is guaranteed to be no larger than is necessary to represent the value.
    ///
    /// \par Example
    ///
    /// To define a byte-sized object using make_elastic_fixed_point and _c:
    /// \snippet snippets.cpp define a byte-sized object using \ref make_elastic_fixed_point and \ref _c

    /// \brief generate an \ref cnl::elastic_fixed_point object of given value
    template<class Narrowest = int, class Integral = int>
    constexpr elastic_fixed_point<std::numeric_limits<Integral>::digits, 0, Narrowest> 
    make_elastic_fixed_point(Integral value)
    {
        return {value};
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // cnl::literals::operator "" _elastic

    /// \brief generate an \ref cnl::elastic_fixed_point object using a literal
    ///
    /// \tparam Digits the characters of the literal sequence
    ///
    /// \return the given value to be represented using an \ref cnl::elastic_fixed_point type
    ///
    /// \note The return type is guaranteed to be no larger
    /// than is necessary to represent the maximum value of Integral.
    ///
    /// \par Example
    ///
    /// To define an int-sized object with value 1536:
    /// \snippet snippets.cpp define an object using elastic literal

    namespace literals {
        template<char... Digits>
        constexpr auto operator "" _elastic()
        -> decltype(make_elastic_fixed_point<int, std::intmax_t, _const_integer_impl::digits_to_integral<Digits...>::value>()) {
            return make_elastic_fixed_point<int, std::intmax_t, _const_integer_impl::digits_to_integral<Digits...>::value>();
        }
    }
}

#endif  // CNL_ELASTIC_FIXED_POINT_H
