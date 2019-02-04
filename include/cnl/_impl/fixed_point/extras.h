
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

#include "to_chars.h"
#include "type.h"
#include "../cmath/abs.h"
#include "../config.h"
#include "../num_traits/fixed_width_scale.h"
#include "../num_traits/unwrap.h"
#include "../unreachable.h"

#include <cmath>
#if defined(CNL_IOSTREAM_ENABLED)
#include <istream>
#include <ostream>
#endif

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

    template<typename Rep, int Exponent, int Radix>
    constexpr auto abs(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    -> decltype(-x)
    {
        return (x>=fixed_point<Rep, Exponent, Radix>{}) ? static_cast<decltype(-x)>(x) : -x;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::sqrt helper functions

    namespace _impl {
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
                                    static_cast<Rep>(bit >> 2),
                                    static_cast<Rep>((result >> 1)+bit))
                     : sqrt_solve3<Rep>(
                                    n,
                                    static_cast<Rep>(bit >> 2),
                                    static_cast<Rep>(result >> 1))
                   : result;
        }

        template<int Exponent>
        struct sqrt_solve1 {
            template<class Rep>
            constexpr Rep operator()(Rep n) const
            {
                using widened_rep = _impl::set_width_t<Rep, _impl::width<Rep>::value*2>;
                return static_cast<Rep>(sqrt_solve3<widened_rep>(
                        _impl::fixed_width_scale<-Exponent>(static_cast<widened_rep>(n)),
                        widened_rep((widened_rep{1}<<((countr_used(n)+1-Exponent)&~1))>>2),
                        widened_rep{0}));
            }
        };
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
    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    sqrt(fixed_point<Rep, Exponent, Radix> const& x)
    {
        using type = fixed_point<Rep, Exponent, Radix>;
        return _impl::to_rep(x)<0
               ? _impl::unreachable<type>("negative value passed to cnl::sqrt")
               : type{_impl::from_rep<type>(_impl::sqrt_solve1<Exponent>{}(unwrap(x)))};
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::floor

    template<class Rep, int Exponent, int Radix,
            _impl::enable_if_t<(Exponent<0), int> dummy = 0>
    constexpr auto floor(fixed_point<Rep, Exponent, Radix> const& x)
    -> decltype(_impl::from_rep<fixed_point<Rep, 0, Radix>>(_impl::to_rep(x)>>constant<-Exponent>())) {
        static_assert(
                Radix==2,
                "cnl::floor(fixed_point<Rep, Exponent, Radix>) not implemented for Exponent<0 && Radix!=2");

        return _impl::from_rep<fixed_point<Rep, 0, Radix>>(_impl::to_rep(x)>>constant<-Exponent>());
    }

    template<class Rep, int Exponent, int Radix>
    constexpr auto floor(fixed_point<Rep, Exponent, Radix> const& x)
    -> _impl::enable_if_t<Exponent>=0, fixed_point<Rep, Exponent, Radix>> {
        return x;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // fixed_point trig functions
    //
    // Placeholder implementations fall back on <cmath> functions which is slow
    // due to conversion to and from floating-point types; also inconvenient as
    // many <cmath> functions are not constexpr.

    namespace _impl {
        template<int NumBits, class Enable = void>
        struct float_of_size;

        template<int NumBits>
        struct float_of_size<NumBits, enable_if_t<NumBits <= sizeof(float)*CHAR_BIT>> {
            using type = float;
        };

        template<int NumBits>
        struct float_of_size<NumBits, enable_if_t<sizeof(float)*CHAR_BIT < NumBits && NumBits <= sizeof(double)*CHAR_BIT>> {
            using type = double;
        };

        template<int NumBits>
        struct float_of_size<NumBits, enable_if_t<sizeof(double)*CHAR_BIT < NumBits && NumBits <= sizeof(long double)*CHAR_BIT>> {
            using type = long double;
        };

        template<class T>
        using float_of_same_size = typename float_of_size<_impl::width<T>::value>::type;

        template<typename Rep, int Exponent, int Radix, _impl::float_of_same_size<Rep>(* F)(
                _impl::float_of_same_size<Rep>)>
        constexpr fixed_point <Rep, Exponent, Radix>
        crib(fixed_point<Rep, Exponent, Radix> const& x) noexcept
        {
            using floating_point = _impl::float_of_same_size<Rep>;
            return static_cast<fixed_point<Rep, Exponent, Radix>>(F(static_cast<floating_point>(x)));
        }
    }

    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    sin(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    {
        return _impl::crib<Rep, Exponent, Radix, std::sin>(x);
    }

    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    cos(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    {
        return _impl::crib<Rep, Exponent, Radix, std::cos>(x);
    }

    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    exp(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    {
        return _impl::crib<Rep, Exponent, Radix, std::exp>(x);
    }

    template<typename Rep, int Exponent, int Radix>
    constexpr fixed_point <Rep, Exponent, Radix>
    pow(fixed_point<Rep, Exponent, Radix> const& x) noexcept
    {
        return _impl::crib<Rep, Exponent, Radix, std::pow>(x);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::fixed_point streaming - (placeholder implementation)

    template<typename Rep, int Exponent, int Radix>
    ::std::ostream& operator<<(::std::ostream& out, fixed_point<Rep, Exponent, Radix> const& fp)
    {
        return out << to_chars(fp).data();
    }

    template<typename Rep, int Exponent, int Radix>
    ::std::istream& operator>>(::std::istream& in, fixed_point <Rep, Exponent, Radix>& fp)
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
    template<typename Rep, int Exponent, int Radix>
    struct numeric_limits<cnl::fixed_point<Rep, Exponent, Radix>>
            : numeric_limits<cnl::_impl::number_base<cnl::fixed_point<Rep, Exponent, Radix>, Rep>> {
        // fixed-point-specific helpers
        using _value_type = cnl::fixed_point<Rep, Exponent, Radix>;
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
    // std::numeric_limits specialization for rounding_integer

    template<typename Rep, int Exponent, int Radix>
    struct numeric_limits<cnl::fixed_point<Rep, Exponent, Radix>>
            : cnl::numeric_limits<cnl::fixed_point<Rep, Exponent, Radix>> {
    };

    template<typename Rep, int Exponent, int Radix>
    struct numeric_limits<cnl::fixed_point<Rep, Exponent, Radix> const>
            : cnl::numeric_limits<cnl::fixed_point<Rep, Exponent, Radix>> {
    };
}

#endif  // CNL_FIXED_POINT_EXTRAS_H
