
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `sg14::elastic` type

#if !defined(SG14_ELASTIC_H)
#define SG14_ELASTIC_H 1

#if ! defined(SG14_GODBOLT_ORG)
#include "elastic_integer.h"
#include <sg14/fixed_point>
#endif

/// study group 14 of the C++ working group
namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // sg14-specific definitions

    /// \brief literal real number approximation that uses fixed-point arithmetic and auto-widens to avoid overflow
    ///
    /// \tparam IntegerDigits the number of integer digits that can be stored
    /// \tparam FractionalDigits the number of fractional digits that can be stored
    /// \tparam Archetype the kind of integer type to use to represent values
    ///
    /// \sa elastic_integer

    template<int IntegerDigits, int FractionalDigits = 0, class Archetype = signed>
    using elastic = fixed_point<elastic_integer<IntegerDigits+FractionalDigits, Archetype>, -FractionalDigits>;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_elastic

    /// \brief generate an \ref sg14::elastic object of given value
    ///
    /// \tparam Archetype the archetype of the resultant \ref sg14::elastic object
    /// \tparam Integral the type of Value
    /// \tparam Value the integer number to be represented
    ///
    /// \return the given value to be represented using an \ref sg14::elastic type
    ///
    /// \note The return type is guaranteed to be no larger than is necessary to represent the value.
    ///
    /// \par Example
    ///
    /// To define an int-sized object using \ref make_elastic and \ref const_integer:
    /// \snippet snippets.cpp define an int-sized object using make_elastic and const_integer

    template<
            typename Archetype = int,
            typename Integral = int,
            Integral Value = 0>
    constexpr auto make_elastic(
            const_integer<Integral, Value> = const_integer<Integral, Value>{})
    -> elastic<
            _const_integer_impl::num_integer_bits(Value),
            -_const_integer_impl::num_integer_zeros(Value),
             Archetype>
    {
        return Value;
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_elastic

    ///
    /// \tparam Archetype the archetype of the resultant \ref sg14::elastic object
    /// \tparam Integral the type of \ref value
    ///
    /// \note The return type is guaranteed to be no larger than is necessary to represent the value.
    ///
    /// \par Example
    ///
    /// To define a byte-sized object using make_elastic and _c:
    /// \snippet snippets.cpp define a byte-sized object using \ref make_elastic and \ref _c

    /// \brief generate an \ref sg14::elastic object of given value
    template<class Archetype = int, class Integral = int>
    constexpr auto make_elastic(Integral value)
    -> elastic<std::numeric_limits<Integral>::digits, 0, Archetype> {
        return {value};
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // sg14::literals::operator "" _elastic

    /// \brief generate an \ref sg14::elastic object using a literal
    ///
    /// \tparam Digits the characters of the literal sequence
    ///
    /// \return the given value to be represented using an \ref sg14::elastic type
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
        -> decltype(make_elastic<int, std::intmax_t, _const_integer_impl::digits_to_integral<Digits...>::value>()) {
            return make_elastic<int, std::intmax_t, _const_integer_impl::digits_to_integral<Digits...>::value>();
        }
    }
}

#endif // SG14_ELASTIC_H
