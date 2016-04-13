
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief supplemental definitions related to the `sg14::fixed_point` type;
/// definitions that straddle two 'homes, e.g. fixed_point.h and cmath, traits or limits;
/// included from sg14/fixed_point.h - do not include directly!

#if !defined(_SG14_FIXED_POINT_UTILS_H)
#define _SG14_FIXED_POINT_UTILS_H 1

#include <cmath>
#include <istream>
#include <limits>

/// study group 14 of the C++ working group
namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    // sg14::abs

    template<class Rep, int Exponent, typename std::enable_if<std::is_signed<Rep>::value, int>::type Dummy = 0>
    constexpr auto abs(const fixed_point <Rep, Exponent>& x) noexcept
    -> _fixed_point_impl::common_type_t<decltype(x), decltype(-x)>
    {
        using common_type = _fixed_point_impl::common_type_t<decltype(x), decltype(-x)>;
        return (x.data()>=0)
               ? static_cast<common_type>(x)
               : static_cast<common_type>(-x);
    }

    template<class Rep, int Exponent, typename std::enable_if<std::is_unsigned<Rep>::value, int>::type Dummy = 0>
    constexpr fixed_point <Rep, Exponent>
    abs(const fixed_point <Rep, Exponent>& x) noexcept
    {
        return x;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::sqrt helper functions

    namespace _fixed_point_impl {
        template<class Rep>
        constexpr Rep sqrt_bit(
                Rep n,
                Rep bit = Rep(1) << (width<Rep>::value-2))
        {
            return (bit>n) ? sqrt_bit<Rep>(n, bit >> 2) : bit;
        }

        template<class Rep>
        constexpr Rep sqrt_solve3(
                Rep n,
                Rep bit,
                Rep result)
        {
            return (bit!=Rep{0})
                   ? (n>=result+bit)
                     ? sqrt_solve3<Rep>(
                                    static_cast<Rep>(n-(result+bit)),
                                    bit >> 2,
                                    static_cast<Rep>((result >> 1)+bit))
                     : sqrt_solve3<Rep>(n, bit >> 2, result >> 1)
                   : result;
        }

        template<class Rep>
        constexpr Rep sqrt_solve1(Rep n)
        {
            return sqrt_solve3<Rep>(n, sqrt_bit<Rep>(n), Rep{0});
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::sqrt

    // https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Binary_numeral_system_.28base_2.29
    // placeholder implementation; slow when calculated at run-time?
    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    sqrt(const fixed_point <Rep, Exponent>& x)
    {
        using widened_type = fixed_point<set_width_t<Rep, width<Rep>::value*2>, Exponent*2>;
        return
#if defined(_SG14_FIXED_POINT_EXCEPTIONS_ENABLED)
                (x<fixed_point<Rep, Exponent>(0))
                ? throw std::invalid_argument("cannot represent square root of negative value") :
#endif
                fixed_point<Rep, Exponent>::from_data(
                        static_cast<Rep>(_fixed_point_impl::sqrt_solve1(widened_type{x}.data())));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::trig
    //
    // Placeholder implementations fall back on <cmath> functions which is slow
    // due to conversion to and from floating-point types; also inconvenient as
    // many <cmath> functions are not constexpr.

    namespace _fixed_point_impl {
        template<class Rep, int Exponent, float_of_same_size<Rep>(* F)(
                float_of_same_size<Rep>)>
        constexpr fixed_point <Rep, Exponent>
        crib(const fixed_point <Rep, Exponent>& x) noexcept
        {
            using floating_point = float_of_same_size<Rep>;
            return static_cast<fixed_point<Rep, Exponent>>(F(static_cast<floating_point>(x)));
        }
    }

    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    sin(const fixed_point <Rep, Exponent>& x) noexcept
    {
        return _fixed_point_impl::crib<Rep, Exponent, std::sin>(x);
    }

    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    cos(const fixed_point <Rep, Exponent>& x) noexcept
    {
        return _fixed_point_impl::crib<Rep, Exponent, std::cos>(x);
    }

    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    exp(const fixed_point <Rep, Exponent>& x) noexcept
    {
        return _fixed_point_impl::crib<Rep, Exponent, std::exp>(x);
    }

    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    pow(const fixed_point <Rep, Exponent>& x) noexcept
    {
        return _fixed_point_impl::crib<Rep, Exponent, std::pow>(x);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::fixed_point streaming - (placeholder implementation)

    template<class Rep, int Exponent>
    ::std::ostream& operator<<(::std::ostream& out, const fixed_point <Rep, Exponent>& fp)
    {
        return out << static_cast<long double>(fp);
    }

    template<class Rep, int Exponent>
    ::std::istream& operator>>(::std::istream& in, fixed_point <Rep, Exponent>& fp)
    {
        long double ld;
        in >> ld;
        fp = ld;
        return in;
    }
}

namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits for sg14::fixed_point

    // note: some members are guessed,
    // some are temporary (assuming rounding style, traps etc.)
    // and some are undefined
    template<class Rep, int Exponent>
    struct numeric_limits<sg14::fixed_point<Rep, Exponent>> {
        // fixed-point-specific helpers
        using _value_type = sg14::fixed_point<Rep, Exponent>;
        using _repr_type = typename _value_type::repr_type;
        using _repr_numeric_limits = numeric_limits<_repr_type>;

        // standard members

        static constexpr bool is_specialized = true;

        static constexpr _value_type min() noexcept
        {
            return _value_type::from_data(1);
        }

        static constexpr _value_type max() noexcept
        {
            return _value_type::from_data(_repr_numeric_limits::max());
        }

        static constexpr _value_type lowest() noexcept
        {
            return _value_type::from_data(_repr_numeric_limits::lowest());
        }

        static constexpr int digits = _repr_numeric_limits::digits;

        //static constexpr int digits10 = ?;
        //static constexpr int max_digits10 = ?;

        static constexpr bool is_signed = _repr_numeric_limits::is_signed;
        static constexpr bool is_integer = _repr_numeric_limits::is_integer;

        // TODO: not entirely certain
        static constexpr bool is_exact = true;

        static constexpr int radix = _repr_numeric_limits::radix;
        static_assert(radix==2, "fixed-point must be represented using binary type");

        static constexpr _value_type epsilon() noexcept
        {
            return _value_type::from_data(1);
        }

        // TODO: not even sure about this when repr_type is built-in integral
        static constexpr _value_type round_error() noexcept
        {
            return static_cast<_value_type>(1);
        }

        // TODO: verify
        static constexpr int min_exponent = _value_type::exponent;
        static constexpr int max_exponent = _value_type::exponent;

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
        static constexpr bool is_modulo = _repr_numeric_limits::is_modulo;

        static constexpr bool traps = _repr_numeric_limits::traps;
        static constexpr bool tinyness_before = false;
        static constexpr float_round_style round_style = _repr_numeric_limits::round_style;
    };
}

#endif	// defined(_SG14_FIXED_POINT_UTILS_H)
