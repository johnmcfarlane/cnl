
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
    /// \tparam IntegerDigits the number of inteteger digits that can be stored
    /// \tparam FractionalDigits the number of fractional digits that can be stored
    /// \tparam Archetype the kind of integer type to use to represent values
    ///
    /// \sa elastic_integer

    template<int IntegerDigits, int FractionalDigits = 0, class Archetype = signed>
    using elastic = fixed_point<elastic_integer<IntegerDigits+FractionalDigits, Archetype>, -FractionalDigits>;

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_elastic

    /// \brief generate an \ref elastic object of given value
    ///
    /// \tparam Value the integer number to be represented
    /// \tparam Archetype the archetype of the resultant \ref elastic object
    ///
    /// \return the given value represented using an \ref elastic type
    ///
    /// \note The return type is guaranteed to be no larger than is necessary to represent the value.
    ///
    /// \par Example
    ///
    /// To define a 1-byte object with value 1024:
    /// \snippet snippets.cpp define a small object using make_elastic
    ///
    /// To define a int-sized object with value 1024:
    /// \snippet snippets.cpp define a fast object using make_elastic

    template<std::int64_t Value, class Archetype = int>
    constexpr auto make_elastic(std::integral_constant<std::int64_t, Value> = std::integral_constant<std::int64_t, Value>())
    -> elastic<_const_integer_impl::num_integer_bits(Value), -_const_integer_impl::num_integer_zeros(Value), Archetype>
    {
        return Value;
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // sg14::literals::operator "" _elastic

    /// \brief generate an \ref sg14::elastic object using a literal
    ///
    /// \return the given value represented using an \ref sg14::elastic type
    ///
    /// \note The return type is guaranteed to be no larger than is necessary to represent the value.
    ///
    /// \par Example
    ///
    /// To define an int-sized object with value 1536:
    /// \snippet snippets.cpp define a small object using elastic literal

    namespace literals {
        template<char... Digits>
        constexpr auto operator "" _elastic()
        -> decltype(make_elastic<_const_integer_impl::digits_to_integral<Digits...>()>())
        {
            return make_elastic<_const_integer_impl::digits_to_integral<Digits...>()>();
        }
    }
}

#endif // SG14_ELASTIC_H
