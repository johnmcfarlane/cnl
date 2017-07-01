
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief supplemental definitions related to the `sg14::fixed_point` type;
/// definitions that straddle two homes, e.g. fixed_point and cmath, traits or limits;
/// included from sg14/fixed_point - do not include directly!

#if !defined(SG14_FIXED_POINT_EXTRAS_H)
#define SG14_FIXED_POINT_EXTRAS_H 1

#include "fixed_point_type.h"

#include <cmath>
#include <istream>

/// study group 14 of the C++ working group
namespace sg14 {

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::_impl::set_rep<fixed_point<>>

    namespace _impl {
        template<class Rep, int Exponent>
        struct get_rep<fixed_point<Rep, Exponent>> {
            using type = Rep;
        };

        template<class OldRep, int Exponent, class NewRep>
        struct set_rep<fixed_point<OldRep, Exponent>, NewRep> {
            using type = fixed_point<NewRep, Exponent>;
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // numeric traits

    template<class Rep, int Exponent>
        struct digits<fixed_point<Rep, Exponent>> : digits<Rep> {
    };

    template<class Rep, int Exponent, _digits_type MinNumBits>
    struct set_digits<fixed_point<Rep, Exponent>, MinNumBits> {
        using type = fixed_point<set_digits_t<Rep, MinNumBits>, Exponent>;
    };

    template<class Rep, int Exponent, class Value>
    struct from_value<fixed_point<Rep, Exponent>, Value> {
        using type = fixed_point<Value>;
    };

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::abs

    template<class Rep, int Exponent>
    constexpr auto abs(const fixed_point<Rep, Exponent>& x) noexcept
    -> decltype(-x)
    {
        return (x >= 0) ? static_cast<decltype(-x)>(x) : -x;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::sqrt helper functions

    namespace _impl {
        namespace fp {
            namespace extras {
                template<class Rep>
                constexpr Rep sqrt_bit(Rep n, Rep bit)
                {
                    return (bit>n) ? sqrt_bit<Rep>(n, bit >> 2) : bit;
                }

                template<class Rep>
                constexpr Rep sqrt_bit(Rep n)
                {
                    return sqrt_bit<Rep>(n, Rep(1) << ((digits<Rep>::value + is_signed<Rep>::value) - 2));
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
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::sqrt

    /// \brief calculates the square root of a \ref fixed_point value
    /// \headerfile sg14/fixed_point
    ///
    /// \param x input parameter
    ///
    /// \return square root of x
    ///
    /// \note This function is a placeholder implementation with poor run-time performance characteristics.
    /// \note It uses
    /// divides the values
    /// without performing any additional scaling or conversion.
    ///
    /// \sa negate, add, subtract, multiply

    // https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Binary_numeral_system_.28base_2.29
    // ?
    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    sqrt(const fixed_point <Rep, Exponent>& x)
    {
        using widened_type = fixed_point<set_digits_t<Rep, digits<Rep>::value*2>, Exponent*2>;
        return
#if defined(SG14_EXCEPTIONS_ENABLED)
                (x<fixed_point<Rep, Exponent>(0))
                ? throw std::invalid_argument("cannot represent square root of negative value") :
#endif
                fixed_point<Rep, Exponent>::from_data(
                        static_cast<Rep>(_impl::fp::extras::sqrt_solve1(widened_type{x}.data())));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // sg14::trig
    //
    // Placeholder implementations fall back on <cmath> functions which is slow
    // due to conversion to and from floating-point types; also inconvenient as
    // many <cmath> functions are not constexpr.

    namespace _impl {
        namespace fp {
            namespace extras {
                template<class Rep, int Exponent, _impl::fp::float_of_same_size<Rep>(* F)(
                        _impl::fp::float_of_same_size<Rep>)>
                constexpr fixed_point <Rep, Exponent>
                crib(const fixed_point <Rep, Exponent>& x) noexcept
                {
                    using floating_point = _impl::fp::float_of_same_size<Rep>;
                    return static_cast<fixed_point<Rep, Exponent>>(F(static_cast<floating_point>(x)));
                }
            }
        }
    }

    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    sin(const fixed_point <Rep, Exponent>& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, std::sin>(x);
    }

    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    cos(const fixed_point <Rep, Exponent>& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, std::cos>(x);
    }

    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    exp(const fixed_point <Rep, Exponent>& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, std::exp>(x);
    }

    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    pow(const fixed_point <Rep, Exponent>& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, std::pow>(x);
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
    struct numeric_limits<sg14::fixed_point<Rep, Exponent>>
            : std::numeric_limits<sg14::_impl::number_base<sg14::fixed_point<Rep, Exponent>, Rep>> {
        // fixed-point-specific helpers
        using _value_type = sg14::fixed_point<Rep, Exponent>;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;

        // standard members

        static constexpr _value_type min() noexcept
        {
            return _value_type::from_data(_rep{1});
        }

        static constexpr _value_type max() noexcept
        {
            return _value_type::from_data(_rep_numeric_limits::max());
        }

        static constexpr _value_type lowest() noexcept
        {
            return _value_type::from_data(_rep_numeric_limits::lowest());
        }

        static constexpr bool is_integer = false;

        static constexpr _value_type epsilon() noexcept
        {
            return _value_type::from_data(_rep{1});
        }

        static constexpr _value_type round_error() noexcept
        {
            return _value_type::from_data(_rep{0});
        }

        static constexpr _value_type infinity() noexcept
        {
            return _value_type::from_data(_rep{0});
        }

        static constexpr _value_type quiet_NaN() noexcept
        {
            return _value_type::from_data(_rep{0});
        }

        static constexpr _value_type signaling_NaN() noexcept
        {
            return _value_type::from_data(_rep{0});
        }

        static constexpr _value_type denorm_min() noexcept
        {
            return _value_type::from_data(_rep{1});
        }
    };
}

#endif	// SG14_FIXED_POINT_EXTRAS_H
