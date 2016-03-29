//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `sg14::elastic` type

#if !defined(_SG14_ELASTIC)
#define _SG14_ELASTIC 1

#include <fixed_point.h>
#include <type_traits.h>

/// study group 14 of the C++ working group
namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // forward-declaration

    // sg14::elastic forward-declaration
    template<int IntegerDigits, int FractionalDigits = 0, class Archetype = signed>
    class elastic;

    namespace _elastic_impl {
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // elastic-specific implementation-dependent definitions

        ////////////////////////////////////////////////////////////////////////////////
        // sg14::_elastic_impl::is_elastic type

        // forward-declaration
        template<class T>
        struct is_elastic;

        // matching true to sg14::elastic
        template<int IntegerDigits, int FractionalDigits, class Archetype>
        struct is_elastic<elastic<IntegerDigits, FractionalDigits, Archetype>>
                : std::true_type {
        };

        // matching false to everything else
        template<class T>
        struct is_elastic
                : std::false_type {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // sg14-specific definitions

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_elastic

    template<typename T>
    struct is_elastic : _elastic_impl::is_elastic<T> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_signed

    template<typename T>
    struct is_signed;

    template<typename T>
    struct is_signed : std::is_signed<T> {
    };

    template<int IntegerDigits, int FractionalDigits, class Archetype>
    struct is_signed<elastic<IntegerDigits, FractionalDigits, Archetype>>
            : sg14::is_signed<Archetype> {
    };

    template<typename ReprType, int Exponent>
    struct is_signed<fixed_point<ReprType, Exponent>>
            : sg14::is_signed<ReprType> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_unsigned

    template<typename T>
    struct is_unsigned : std::integral_constant<bool, !is_signed<T>::value> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_signed

    template<typename T>
    struct make_signed;

    template<int IntegerDigits, int FractionalDigits, class Archetype>
    struct make_signed<elastic<IntegerDigits, FractionalDigits, Archetype>> {
        using type = elastic<IntegerDigits, FractionalDigits, typename sg14::make_signed<Archetype>::type>;
    };

    template<typename T>
    struct make_signed : std::make_signed<T> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_unsigned

    template<typename T>
    struct make_unsigned;

    template<int IntegerDigits, int FractionalDigits, class Archetype>
    struct make_unsigned<elastic<IntegerDigits, FractionalDigits, Archetype>> {
        using type = elastic<IntegerDigits, FractionalDigits, typename sg14::make_unsigned<Archetype>::type>;
    };

    template<typename T>
    struct make_unsigned : std::make_unsigned<T> {
    };

    /// \brief general-purpose real number approximation that avoids overflow
    ///
    /// \tparam IntegerDigits the number of integer bits of storage
    /// \tparam FractionalDigits the number of fractional bits of storage
    /// \tparam Archetype a type like the one used to represent to store this value
    ///
    /// \note This type illustrates an application of @ref fixed_point as put forth in
    /// <a href="http://johnmcfarlane.github.io/fixed_point/papers/elastic.html">this paper</a>.

    template<int IntegerDigits, int FractionalDigits, class Archetype>
    class elastic {
        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // constants

    public:
        /// see `IntegerDigits`
        static constexpr int integer_digits = IntegerDigits;

        /// the number of fractional bits of storage
        static constexpr int fractional_digits = FractionalDigits;

    private:
        static constexpr int exponent = -fractional_digits;

        static constexpr int digits = integer_digits+fractional_digits;
        static_assert(digits>0, "sum of integer and fractional digits must be positive");

        static constexpr int bits = digits+is_signed<Archetype>::value;
        static constexpr int bytes = (bits+CHAR_BIT-1)/CHAR_BIT;

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // types

    public:
        /// a type like the one used to represent to store this value
        using archetype = Archetype;

        /// \private implementation-specific
        using _integer_type = typename sg14::resize<archetype, bytes>::type;

        /// \private implementation-specific
        using _fixed_point_type = sg14::fixed_point<_integer_type, exponent>;

    private:
        template<typename Integer>
        static constexpr bool is_relative()
        {
            return std::is_same<sg14::resize<Integer, sizeof(archetype)>, archetype>::value;
        }

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // functions

        ////////////////////////////////////////////////////////////////////////////////
        // constructors

    public:
        /// default constructor (zero-inializes value)
        explicit constexpr elastic() noexcept
                :_value{0.} { }

        /// copy constructor (guaranteed never to throw)
        template<int RhsIntegerDigits, int RhsFractionalDigits, class RhsArchetype>
        explicit constexpr elastic(const elastic<RhsIntegerDigits, RhsFractionalDigits, RhsArchetype>& rhs)
                :_value(rhs._data()) { }

        /// constructor taking fixed_point type
        template<typename RhsReprType, int RhsExponent>
        constexpr elastic(const fixed_point<RhsReprType, RhsExponent>& value)
                :_value(value) { }

        /// constructor converting from any other type
        template<class Rhs/*, typename = typename std::enable_if<std::is_integral<Integer>::value>::type*/>
        explicit constexpr elastic(const Rhs& rhs)
                :elastic(static_cast<_fixed_point_type>(rhs)) { }

        ////////////////////////////////////////////////////////////////////////////////
        // leakage (not part of the API)

        // private member accessor; do not use!
        constexpr const _fixed_point_type& _data() const noexcept
        {
            return _value;
        }

    private:
        _fixed_point_type _value;
    };

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // sg14::elastic operator overloads

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::elastic comparison operators

    template<
            class LhsElastic,
            class RhsElastic,
            typename = std::enable_if<sg14::is_elastic<LhsElastic>::value && sg14::is_elastic<LhsElastic>::value>>
    constexpr auto operator<(const LhsElastic& lhs, const RhsElastic& rhs)
    -> decltype(lhs._data()<rhs._data())
    {
        return lhs._data()<rhs._data();
    }

    template<
            class LhsElastic,
            class RhsElastic,
            typename = std::enable_if<sg14::is_elastic<LhsElastic>::value && sg14::is_elastic<LhsElastic>::value>>
    constexpr auto operator>(const LhsElastic& lhs, const RhsElastic& rhs)
    -> decltype(lhs._data()>rhs._data())
    {
        return lhs._data()>rhs._data();
    }

    template<
            class LhsElastic,
            class RhsElastic,
            typename = std::enable_if<sg14::is_elastic<LhsElastic>::value && sg14::is_elastic<LhsElastic>::value>>
    constexpr auto operator<=(const LhsElastic& lhs, const RhsElastic& rhs)
    -> decltype(lhs._data()<=rhs._data())
    {
        return lhs._data()<=rhs._data();
    }

    template<
            class LhsElastic,
            class RhsElastic,
            typename = std::enable_if<sg14::is_elastic<LhsElastic>::value && sg14::is_elastic<LhsElastic>::value>>
    constexpr auto operator>=(const LhsElastic& lhs, const RhsElastic& rhs)
    -> decltype(lhs._data()>=rhs._data())
    {
        return lhs._data()>=rhs._data();
    }

    template<
            class LhsElastic,
            class RhsElastic,
            typename = std::enable_if<sg14::is_elastic<LhsElastic>::value && sg14::is_elastic<LhsElastic>::value>>
    constexpr auto operator==(const LhsElastic& lhs, const RhsElastic& rhs)
    -> decltype(lhs._data()==rhs._data())
    {
        return lhs._data()==rhs._data();
    }

    template<
            class LhsElastic,
            class RhsElastic,
            typename = std::enable_if<sg14::is_elastic<LhsElastic>::value && sg14::is_elastic<LhsElastic>::value>>
    constexpr auto operator!=(const LhsElastic& lhs, const RhsElastic& rhs)
    -> decltype(lhs._data()!=rhs._data())
    {
        return lhs._data()!=rhs._data();
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::elastic arithmetic operators

    // negate
    template<int RhsIntegerDigits, int RhsFractionalDigits, class RhsArchetype>
    constexpr auto operator-(const elastic<RhsIntegerDigits, RhsFractionalDigits, RhsArchetype>& rhs)
    -> elastic<RhsIntegerDigits, RhsFractionalDigits, typename sg14::make_signed<RhsArchetype>::type>
    {
        using result_archetype = typename sg14::make_signed<RhsArchetype>::type;
        using result_type = elastic<RhsIntegerDigits, RhsFractionalDigits, result_archetype>;
        using result_fixed_point_type = typename result_type::_fixed_point_type;

        return result_type{-static_cast<result_fixed_point_type>(rhs._data())};
    }
}

#endif //_SG14_ELASTIC
