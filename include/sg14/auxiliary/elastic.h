
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

    // sg14::make_elastic helper definitions
    namespace _elastic_impl {
        template<class Integer>
        constexpr int num_integer_bits(Integer value)
        {
            return value ? 1+num_integer_bits(value/2) : 0;
        }

        template<class Integer>
        constexpr int num_fractional_bits(Integer value)
        {
            return (((value/2)*2)==value) ? num_fractional_bits(value/2)-1 : 0;
        }

        template<class Integer, Integer Value, class Archetype, class Enabled = void>
        struct elastication;

        template<class Integer, Integer Value, class Archetype>
        struct elastication<Integer, Value, Archetype, typename std::enable_if<Value==0>::type> {
            using type = elastic<1, 0, typename make_unsigned<Archetype>::type>;
        };

        template<class Integer, Integer Value, class Archetype>
        struct elastication<Integer, Value, Archetype, typename std::enable_if<Value!=0>::type> {
            static_assert(std::is_integral<Integer>::value, "template parameter, Integer, is not integral");

            using archetype = typename std::conditional<(Value>=0),
                    typename make_unsigned<Archetype>::type,
                    typename make_signed<Archetype>::type>::type;

            using type = elastic<
                    sg14::_impl::max(1, num_integer_bits(Value)),
                    num_fractional_bits(Value),
                    archetype>;
        };

        template<class Integer, Integer Value, class Archetype>
        using make_elastic_t = typename _elastic_impl::elastication<Integer, Value, Archetype>::type;
    }

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
    constexpr auto make_elastic()
    -> _elastic_impl::make_elastic_t<std::int64_t, Value, Archetype>
    {
        return _elastic_impl::make_elastic_t<std::int64_t, Value, Archetype>{Value};
    }
}

#endif // SG14_ELASTIC_H
