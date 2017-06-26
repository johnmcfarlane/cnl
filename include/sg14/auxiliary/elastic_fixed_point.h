
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `sg14::elastic_fixed_point` type

#if !defined(SG14_ELASTIC_FIXED_POINT_H)
#define SG14_ELASTIC_FIXED_POINT_H 1

#include "elastic_integer.h"
#include <sg14/fixed_point>

/// study group 14 of the C++ working group
namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // sg14-specific definitions

    /// \brief literal real number approximation that uses fixed-point arithmetic and auto-widens to avoid overflow
    ///
    /// \tparam IntegerDigits the number of integer digits that can be stored
    /// \tparam FractionalDigits the number of fractional digits that can be stored
    /// \tparam Narrowest the most narrow integer type to use to represent values
    ///
    /// \sa elastic_integer

    template<int IntegerDigits, int FractionalDigits = 0, class Narrowest = signed>
    using elastic_fixed_point = fixed_point<elastic_integer<IntegerDigits+FractionalDigits, Narrowest>, -FractionalDigits>;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_elastic_fixed_point

    /// \brief generate an \ref sg14::elastic_fixed_point object of given value
    ///
    /// \tparam Narrowest the narrowest type to use as storage
    /// in the resultant \ref sg14::elastic_fixed_point object
    /// \tparam Integral the type of Value
    /// \tparam Value the integer number to be represented
    ///
    /// \return the given value to be represented using an \ref sg14::elastic_fixed_point type
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
    constexpr elastic_fixed_point<_impl::max(_impl::used_bits_symmetric(Value), 1), -trailing_bits(Value), Narrowest>
    make_elastic_fixed_point(const_integer<Integral, Value> = const_integer<Integral, Value>{})
    {
        return Value;
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_elastic_fixed_point

    ///
    /// \tparam Narrowest the most narrow storage type of the resultant \ref sg14::elastic_fixed_point object
    /// \tparam Integral the type of \ref value
    ///
    /// \note The return type is guaranteed to be no larger than is necessary to represent the value.
    ///
    /// \par Example
    ///
    /// To define a byte-sized object using make_elastic_fixed_point and _c:
    /// \snippet snippets.cpp define a byte-sized object using \ref make_elastic_fixed_point and \ref _c

    /// \brief generate an \ref sg14::elastic_fixed_point object of given value
    template<class Narrowest = int, class Integral = int>
    constexpr elastic_fixed_point<std::numeric_limits<Integral>::digits, 0, Narrowest> 
    make_elastic_fixed_point(Integral value)
    {
        return {value};
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // sg14::literals::operator "" _elastic

    /// \brief generate an \ref sg14::elastic_fixed_point object using a literal
    ///
    /// \tparam Digits the characters of the literal sequence
    ///
    /// \return the given value to be represented using an \ref sg14::elastic_fixed_point type
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

#endif // SG14_ELASTIC_FIXED_POINT_H
