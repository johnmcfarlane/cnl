
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief supplemental definitions related to the `sg14::fixed_point` type

#if !defined(_SG14_FIXED_POINT_UTILS_H)
#define _SG14_FIXED_POINT_UTILS_H 1

#include "fixed_point.h"

#include <cmath>
#include <istream>
#include <limits>
#include <ostream>

/// study group 14 of the C++ working group
namespace sg14 {
    ////////////////////////////////////////////////////////////////////////////////
    // additional traits for sg14::fixed_point

    /// is_fixed_point
    template<class T>
    using is_fixed_point = _impl::is_fixed_point<T>;

    // is_real
    template<class T>
    using is_real = std::integral_constant<
            bool,
            std::is_floating_point<T>::value || sg14::is_fixed_point<T>::value>;

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::abs

    template<class ReprType, int Exponent, typename std::enable_if<std::is_signed<ReprType>::value, int>::type Dummy = 0>
    constexpr fixed_point <ReprType, Exponent>
    abs(const fixed_point <ReprType, Exponent>& x) noexcept
    {
        return (x.data()>=0) ? x : -x;
    }

    template<class ReprType, int Exponent, typename std::enable_if<std::is_unsigned<ReprType>::value, int>::type Dummy = 0>
    constexpr fixed_point <ReprType, Exponent>
    abs(const fixed_point <ReprType, Exponent>& x) noexcept
    {
        return x;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::trig
    //
    // Placeholder implementations fall back on <cmath> functions which is slow
    // due to conversion to and from floating-point types; also inconvenient as
    // many <cmath> functions are not constexpr.

    namespace _impl {
        template<class ReprType, int Exponent, _impl::make_float<sizeof(ReprType)>(* F)(
                _impl::make_float<sizeof(ReprType)>)>
        constexpr fixed_point <ReprType, Exponent>
        crib(const fixed_point <ReprType, Exponent>& x) noexcept
        {
            using floating_point = _impl::make_float<sizeof(ReprType)>;
            return static_cast<fixed_point<ReprType, Exponent>>(F(static_cast<floating_point>(x)));
        }
    }

    template<class ReprType, int Exponent>
    constexpr fixed_point <ReprType, Exponent>
    sin(const fixed_point <ReprType, Exponent>& x) noexcept
    {
        return _impl::crib<ReprType, Exponent, std::sin>(x);
    }

    template<class ReprType, int Exponent>
    constexpr fixed_point <ReprType, Exponent>
    cos(const fixed_point <ReprType, Exponent>& x) noexcept
    {
        return _impl::crib<ReprType, Exponent, std::cos>(x);
    }

    template<class ReprType, int Exponent>
    constexpr fixed_point <ReprType, Exponent>
    exp(const fixed_point <ReprType, Exponent>& x) noexcept
    {
        return _impl::crib<ReprType, Exponent, std::exp>(x);
    }

    template<class ReprType, int Exponent>
    constexpr fixed_point <ReprType, Exponent>
    pow(const fixed_point <ReprType, Exponent>& x) noexcept
    {
        return _impl::crib<ReprType, Exponent, std::pow>(x);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::fixed_point streaming - (placeholder implementation)

    template<class ReprType, int Exponent>
    ::std::ostream& operator<<(::std::ostream& out, const fixed_point <ReprType, Exponent>& fp)
    {
        return out << static_cast<long double>(fp);
    }

    template<class ReprType, int Exponent>
    ::std::istream& operator>>(::std::istream& in, fixed_point <ReprType, Exponent>& fp)
    {
        long double ld;
        in >> ld;
        fp = ld;
        return in;
    }
}

namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits for fixed-point

    // note: some members are guessed,
    // some are temporary (assuming rounding style, traps etc.)
    // and some are undefined
    template<class ReprType, int Exponent>
    struct numeric_limits<sg14::fixed_point<ReprType, Exponent>> {
        // fixed-point-specific helpers
        using _value_type = sg14::fixed_point<ReprType, Exponent>;
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
        static constexpr bool is_integer = false;

        // TODO: not entirely certain
        static constexpr bool is_exact = true;

        static constexpr int radix = _repr_numeric_limits::radix;
        static_assert(radix==2, "fixed-point must be represented using binary type");

        static constexpr _value_type epsilon() noexcept
        {
            return _value_type::from_data(1);
        }

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

#if defined(_GLIBCXX_USE_INT128)
// std type specializations required to promote 128-bit integers to first-class integers under GCC/Clang
namespace std {
    // std::is_integral - related to https://llvm.org/bugs/show_bug.cgi?id=23156
    template<>
    struct is_integral<__int128> : std::true_type {
    };

    template<>
    struct is_integral<unsigned __int128> : std::true_type {
    };

    // std::is_signed
    template<>
    struct is_signed<__int128> : std::true_type {
    };

    template<>
    struct is_signed<unsigned __int128> : std::false_type {
    };

    // std::is_signed
    template<>
    struct is_unsigned<__int128> : std::false_type {
    };

    template<>
    struct is_unsigned<unsigned __int128> : std::true_type {
    };

    // std::make_signed
    template<>
    struct make_signed<__int128> {
        using type = __int128;
    };

    template<>
    struct make_signed<unsigned __int128> {
        using type = __int128;
    };

    // std::make_signed
    template<>
    struct make_unsigned<__int128> {
        using type = unsigned __int128;
    };

    template<>
    struct make_unsigned<unsigned __int128> {
        using type = __int128;
    };
}
#endif

#endif	// defined(_SG14_FIXED_POINT_UTILS_H)
