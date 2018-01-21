
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief supplemental definitions related to the `cnl::fixed_point` type;
/// definitions that straddle two homes, e.g. fixed_point and cmath, traits or limits;
/// included from cnl/fixed_point.h - do not include directly!

#if !defined(CNL_FIXED_POINT_EXTRAS_H)
#define CNL_FIXED_POINT_EXTRAS_H 1

#include "type.h"

#include <cmath>
#include <istream>

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::abs

    /// \brief absolute value
    /// \headerfile cnl/fixed_point.h
    ///
    /// \param x input parameter
    ///
    /// \return `|x|`
    ///
    /// \sa \ref std::vector

    template<class Rep, int Exponent>
    constexpr auto abs(fixed_point<Rep, Exponent> const& x) noexcept
    -> decltype(-x)
    {
        return (x >= 0) ? static_cast<decltype(-x)>(x) : -x;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::sqrt helper functions

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
                    return sqrt_bit<Rep>(n, Rep(1) << (Rep((digits<Rep>::value + is_signed<Rep>::value) - 2)));
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

                struct sqrt_solve1 {
                    template<class Rep>
                    constexpr Rep operator()(Rep n) const
                    {
                        return sqrt_solve3<Rep>(n, sqrt_bit<Rep>(n), Rep{0});
                    }
                };
            }
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::sqrt

    /// \brief calculates the square root of a \ref fixed_point value
    /// \headerfile cnl/fixed_point.h
    ///
    /// \param x input parameter
    ///
    /// \return square root of x
    ///
    /// \note This function is a placeholder implementation with poor run-time performance characteristics.
    ///
    /// \sa multiply

    // https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Binary_numeral_system_.28base_2.29
    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    sqrt(fixed_point<Rep, Exponent> const& x)
    {
        using widened_rep = set_digits_t<Rep, digits<Rep>::value*2>;
        return
#if defined(CNL_EXCEPTIONS_ENABLED)
                (x<_impl::from_rep<fixed_point<Rep, Exponent>>(0))
                ? throw std::invalid_argument("cannot represent square root of negative value") :
#endif
                _impl::from_rep<fixed_point<Rep, Exponent>>(for_rep<widened_rep>(
                        _impl::fp::extras::sqrt_solve1(),
                        _impl::scale<-Exponent>(static_cast<widened_rep>(_impl::to_rep(x)))));
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::trig
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
                crib(fixed_point<Rep, Exponent> const& x) noexcept
                {
                    using floating_point = _impl::fp::float_of_same_size<Rep>;
                    return static_cast<fixed_point<Rep, Exponent>>(F(static_cast<floating_point>(x)));
                }
            }
        }
    }

    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    sin(fixed_point<Rep, Exponent> const& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, std::sin>(x);
    }

    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    cos(fixed_point<Rep, Exponent> const& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, std::cos>(x);
    }

    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    exp(fixed_point<Rep, Exponent> const& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, std::exp>(x);
    }

    template<class Rep, int Exponent>
    constexpr fixed_point <Rep, Exponent>
    pow(fixed_point<Rep, Exponent> const& x) noexcept
    {
        return _impl::fp::extras::crib<Rep, Exponent, std::pow>(x);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::fixed_point streaming - (placeholder implementation)

    template<class Rep, int Exponent>
    ::std::ostream& operator<<(::std::ostream& out, fixed_point<Rep, Exponent> const& fp)
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

namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits for cnl::fixed_point

    // note: some members are guessed,
    // some are temporary (assuming rounding style, traps etc.)
    // and some are undefined
    template<class Rep, int Exponent>
    struct numeric_limits<cnl::fixed_point<Rep, Exponent>>
            : numeric_limits<cnl::_impl::number_base<cnl::fixed_point<Rep, Exponent>, Rep>> {
        // fixed-point-specific helpers
        using _value_type = cnl::fixed_point<Rep, Exponent>;
        using _rep = typename _value_type::rep;
        using _rep_numeric_limits = numeric_limits<_rep>;

        // standard members

        static constexpr _value_type min() noexcept
        {
            return _impl::from_rep<_value_type>(_rep{1});
        }

        static constexpr _value_type max() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::max());
        }

        static constexpr _value_type lowest() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::lowest());
        }

        static constexpr bool is_integer = false;

        static constexpr _value_type epsilon() noexcept
        {
            return _impl::from_rep<_value_type>(_rep{1});
        }

        static constexpr _value_type round_error() noexcept
        {
            return _impl::from_rep<_value_type>(_rep{0});
        }

        static constexpr _value_type infinity() noexcept
        {
            return _impl::from_rep<_value_type>(_rep{0});
        }

        static constexpr _value_type quiet_NaN() noexcept
        {
            return _impl::from_rep<_value_type>(_rep{0});
        }

        static constexpr _value_type signaling_NaN() noexcept
        {
            return _impl::from_rep<_value_type>(_rep{0});
        }

        static constexpr _value_type denorm_min() noexcept
        {
            return _impl::from_rep<_value_type>(_rep{1});
        }
    };
}

namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits specialization for precise_integer

    template<class Rep, int Exponent>
    struct numeric_limits<cnl::fixed_point<Rep, Exponent>>
            : cnl::numeric_limits<cnl::fixed_point<Rep, Exponent>> {
    };

    template<class Rep, int Exponent>
    struct numeric_limits<cnl::fixed_point<Rep, Exponent> const>
            : cnl::numeric_limits<cnl::fixed_point<Rep, Exponent>> {
    };
}

#endif  // CNL_FIXED_POINT_EXTRAS_H
