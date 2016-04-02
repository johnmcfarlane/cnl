//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `sg14::elastic` type

#if !defined(_SG14_ELASTIC)
#define _SG14_ELASTIC 1

#include <fixed_point_utils.h>
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

    template<class T>
    struct is_elastic : _elastic_impl::is_elastic<T> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // sg14:elastic specializations of std templates

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_signed

    template<class T>
    struct is_signed;

    template<class T>
    struct is_signed : std::is_signed<T> {
    };

    template<int IntegerDigits, int FractionalDigits, class Archetype>
    struct is_signed<elastic<IntegerDigits, FractionalDigits, Archetype>>
            : sg14::is_signed<Archetype> {
    };

    template<class ReprType, int Exponent>
    struct is_signed<fixed_point<ReprType, Exponent>>
            : sg14::is_signed<ReprType> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::is_unsigned

    template<class T>
    struct is_unsigned : std::integral_constant<bool, !is_signed<T>::value> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_signed

    template<class T>
    struct make_signed;

    template<int IntegerDigits, int FractionalDigits, class Archetype>
    struct make_signed<elastic<IntegerDigits, FractionalDigits, Archetype>> {
        using type = elastic<IntegerDigits, FractionalDigits, typename sg14::make_signed<Archetype>::type>;
    };

    template<class T>
    struct make_signed : std::make_signed<T> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_signed_t

    template<class T>
    using make_signed_t = typename make_signed<T>::type;

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_unsigned

    template<class T>
    struct make_unsigned;

    template<int IntegerDigits, int FractionalDigits, class Archetype>
    struct make_unsigned<elastic<IntegerDigits, FractionalDigits, Archetype>> {
        using type = elastic<IntegerDigits, FractionalDigits, typename sg14::make_unsigned<Archetype>::type>;
    };

    template<class T>
    struct make_unsigned : std::make_unsigned<T> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::make_unsigned_t

    template<class T>
    using make_unsigned_t = typename make_unsigned<T>::type;

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
        // template parameter definitions

    public:
        /// see \a IntegerDigits
        static constexpr int integer_digits = IntegerDigits;

        /// see `FractionalDigits`
        static constexpr int fractional_digits = FractionalDigits;

        /// a type like the one used to represent to store this value
        using archetype = Archetype;

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // digit count constants

    private:
        static constexpr int exponent = -fractional_digits;

        static constexpr int digits = integer_digits+fractional_digits;
        static_assert(digits>=0, "sum of integer and fractional digits must be positive");

        static constexpr int bits = digits+is_signed<Archetype>::value;
        static constexpr int bytes = (bits+CHAR_BIT-1)/CHAR_BIT;

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // underlying interger type-related definitions

    public:
        /// \private implementation-specific
        using _integer_type = typename sg14::resize<archetype, bytes>::type;

        /// \private implementation-specific
        using _fixed_point_type = sg14::fixed_point<_integer_type, exponent>;

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // value constants

        ////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////
        // functions

    private:
        template<class Integer>
        static constexpr bool is_relative()
        {
            return std::is_same<sg14::resize<Integer, sizeof(archetype)>, archetype>::value;
        }

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
        template<class RhsReprType, int RhsExponent>
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

    // TODO: ponder this specialization
    //template<class Archetype>
    //class elastic<0, 0, Archetype> {
    //};

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
    -> elastic<RhsIntegerDigits, RhsFractionalDigits, make_signed_t<RhsArchetype>>
    {
        using result_archetype = make_signed_t<RhsArchetype>;
        using result_type = elastic<RhsIntegerDigits, RhsFractionalDigits, result_archetype>;
        using result_fixed_point_type = typename result_type::_fixed_point_type;

        return result_type{-static_cast<result_fixed_point_type>(rhs._data())};
    }

    // add
    namespace _elastic_impl {
        template<
                int LhsIntegerDigits, int LhsFractionalDigits, class LhsArchetype,
                int RhsIntegerDigits, int RhsFractionalDigits, class RhsArchetype>
        using add_result_type = elastic<
                sg14::_impl::max(LhsIntegerDigits, RhsIntegerDigits)+1,
                sg14::_impl::max(LhsFractionalDigits, RhsFractionalDigits),
                typename std::conditional<
                        sg14::is_signed<LhsArchetype>::value || sg14::is_signed<RhsArchetype>::value,
                        make_signed_t<LhsArchetype>,
                        make_unsigned_t<RhsArchetype>>::type>;
    }

    template<
            int LhsIntegerDigits, int LhsFractionalDigits, class LhsArchetype,
            int RhsIntegerDigits, int RhsFractionalDigits, class RhsArchetype>
    constexpr auto operator+(
            const elastic<LhsIntegerDigits, LhsFractionalDigits, LhsArchetype>& lhs,
            const elastic<RhsIntegerDigits, RhsFractionalDigits, RhsArchetype>& rhs)
    -> _elastic_impl::add_result_type<LhsIntegerDigits, LhsFractionalDigits, LhsArchetype, RhsIntegerDigits, RhsFractionalDigits, RhsArchetype>
    {
        using result_type = _elastic_impl::add_result_type<LhsIntegerDigits, LhsFractionalDigits, LhsArchetype, RhsIntegerDigits, RhsFractionalDigits, RhsArchetype>;
        using fixed_point_result_type = typename result_type::_fixed_point_type;

        return sg14::add<fixed_point_result_type>(lhs._data(), rhs._data());

    }
}

namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits for sg14::elastic

    // note: some members are guessed,
    // some are temporary (assuming rounding style, traps etc.)
    // and some are undefined
    template<int IntegerDigits, int FractionalDigits, class Archetype>
    struct numeric_limits<sg14::elastic<IntegerDigits, FractionalDigits, Archetype>> {
        // fixed-point-specific helpers
        using _value_type = sg14::elastic<IntegerDigits, FractionalDigits, Archetype>;
        using _fixed_point_type = typename _value_type::_fixed_point_type;
        using _fixed_point_limits = numeric_limits<_fixed_point_type>;
        static_assert(_fixed_point_limits::is_specialized,
                "specialization of sg14::fixed_point<> is necessary for specialization of sg14::elastic<>");
        using _fixed_point_repr_type = typename _fixed_point_type::repr_type;

        // standard members

        static constexpr bool is_specialized = true;

        static constexpr _value_type min() noexcept
        {
            return _fixed_point_limits::min();
        }

        static constexpr _value_type max() noexcept
        {
            return _fixed_point_type::from_data(
                    numeric_limits<_fixed_point_repr_type>::max()
                            >> (numeric_limits<_fixed_point_repr_type>::digits-digits));
        }

        static constexpr _value_type lowest() noexcept
        {
            return _fixed_point_type::from_data(
                    numeric_limits<_fixed_point_repr_type>::lowest()
                            >> (numeric_limits<_fixed_point_repr_type>::digits-digits));
        }

        static constexpr int digits = _value_type::integer_digits+_value_type::fractional_digits;

        //static constexpr int digits10 = ?;
        //static constexpr int max_digits10 = ?;

        static constexpr bool is_signed = _fixed_point_limits::is_signed;
        static constexpr bool is_integer = _value_type::fractional_digits<=0;

        // TODO: not entirely certain
        static constexpr bool is_exact = true;

        static constexpr int radix = _fixed_point_limits::radix;
        static_assert(radix==2, "fixed-point must be represented using binary type");

        static constexpr _value_type epsilon() noexcept
        {
            return _value_type::from_data(1);
        }

        static constexpr _value_type round_error() noexcept
        {
            return _fixed_point_limits::round_error();
        }

        // TODO: verify
        static constexpr int min_exponent = _fixed_point_type::exponent;
        static constexpr int max_exponent = _fixed_point_type::exponent;

        //static constexpr int min_exponent10 = ?;
        //static constexpr int max_exponent10 = ?;

        static constexpr bool has_infinity = false;
        static constexpr bool has_quiet_NaN = false;
        static constexpr bool has_signaling_NaN = false;
        static constexpr float_denorm_style has_denorm = denorm_absent;
        static constexpr bool has_denorm_loss = false;

        static constexpr _value_type infinity() noexcept
        {
            return static_cast<_value_type>(0);
        }

        static constexpr _value_type quiet_NaN() noexcept
        {
            return static_cast<_value_type>(0);
        }

        static constexpr _value_type signaling_NaN() noexcept
        {
            return static_cast<_value_type>(0);
        }

        static constexpr _value_type denorm_min() noexcept
        {
            return static_cast<_value_type>(0);
        }

        static constexpr bool is_iec559 = false;
        static constexpr bool is_bounded = true;
        static constexpr bool is_modulo = _fixed_point_limits::is_modulo;

        static constexpr bool traps = _fixed_point_limits::traps;
        static constexpr bool tinyness_before = false;
        static constexpr float_round_style round_style = _fixed_point_limits::round_style;
    };
}

#endif //_SG14_ELASTIC
