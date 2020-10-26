
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief supplemental definitions related to the `cnl::scaled_integer` type;
/// definitions that straddle two homes, e.g. scaled_integer and cmath, traits or limits;
/// included from cnl/scaled_integer.h - do not include directly!

#if !defined(CNL_IMPL_SCALED_INTEGER_EXTRAS_H)
#define CNL_IMPL_SCALED_INTEGER_EXTRAS_H

#include "../cmath/abs.h"
#include "../config.h"
#include "../num_traits/fixed_width_scale.h"
#include "../num_traits/set_width.h"
#include "../num_traits/unwrap.h"
#include "../num_traits/width.h"
#include "../unreachable.h"
#include "to_chars.h"
#include "type.h"

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
    /// \headerfile cnl/scaled_integer.h
    ///
    /// \param x input parameter
    ///
    /// \return `|x|`
    ///
    /// \sa \ref std::abs

    template<typename Rep, class Scale>
    CNL_NODISCARD constexpr auto abs(scaled_integer<Rep, Scale> const& x) noexcept
    -> decltype(-x)
    {
        return (x>=scaled_integer<Rep, Scale>{}) ? static_cast<decltype(-x)>(x) : -x;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::sqrt helper functions

    namespace _impl {
        template<class Rep>
        CNL_NODISCARD constexpr Rep sqrt_solve3(
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
            CNL_NODISCARD constexpr Rep operator()(Rep n) const
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

    /// \brief calculates the square root of a \ref scaled_integer value
    /// \headerfile cnl/scaled_integer.h
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
    CNL_NODISCARD constexpr auto
    sqrt(scaled_integer<Rep, power<Exponent, Radix>> const& x)
    -> scaled_integer<Rep, power<Exponent, Radix>>
    {
        using type = scaled_integer<Rep, power<Exponent, Radix>>;
        return _impl::to_rep(x)<0
               ? _impl::unreachable<type>("negative value passed to cnl::sqrt")
               : type{_impl::from_rep<type>(_impl::sqrt_solve1<Exponent>{}(unwrap(x)))};
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::floor

    template<class Rep, int Exponent, int Radix,
            _impl::enable_if_t<(Exponent<0), int> = 0>
    CNL_NODISCARD constexpr auto floor(scaled_integer<Rep, power<Exponent, Radix>> const& x) {
        static_assert(
                Radix==2,
                "cnl::floor(scaled_integer<Rep, power<Exponent, Radix>>) not implemented for Exponent<0 && Radix!=2");

        return _impl::from_rep<scaled_integer<Rep, power<0, Radix>>>(_impl::to_rep(x)>>constant<-Exponent>());
    }

    template<class Rep, int Exponent, int Radix>
    CNL_NODISCARD constexpr auto floor(scaled_integer<Rep, power<Exponent, Radix>> const& x)
    -> _impl::enable_if_t<Exponent>=0, scaled_integer<Rep, power<Exponent, Radix>>> {
        return x;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // scaled_integer trig functions
    //
    // Placeholder implementations fall back on <cmath> functions which is slow
    // due to conversion to and from floating-point types; also inconvenient as
    // many <cmath> functions are not CNL_NODISCARD constexpr.

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
        CNL_NODISCARD constexpr scaled_integer<Rep, power<Exponent, Radix>>
        crib(scaled_integer<Rep, power<Exponent, Radix>> const& x) noexcept
        {
            using floating_point = _impl::float_of_same_size<Rep>;
            return static_cast<scaled_integer<Rep, power<Exponent, Radix>>>(F(static_cast<floating_point>(x)));
        }
    }

    template<typename Rep, int Exponent, int Radix>
    CNL_NODISCARD constexpr scaled_integer<Rep, power<Exponent, Radix>>
    sin(scaled_integer<Rep, power<Exponent, Radix>> const& x) noexcept
    {
        return _impl::crib<Rep, Exponent, Radix, std::sin>(x);
    }

    template<typename Rep, int Exponent, int Radix>
    CNL_NODISCARD constexpr scaled_integer<Rep, power<Exponent, Radix>>
    cos(scaled_integer<Rep, power<Exponent, Radix>> const& x) noexcept
    {
        return _impl::crib<Rep, Exponent, Radix, std::cos>(x);
    }

    template<typename Rep, int Exponent, int Radix>
    CNL_NODISCARD constexpr scaled_integer<Rep, power<Exponent, Radix>>
    exp(scaled_integer<Rep, power<Exponent, Radix>> const& x) noexcept
    {
        return _impl::crib<Rep, Exponent, Radix, std::exp>(x);
    }

    template<typename Rep, int Exponent, int Radix>
    CNL_NODISCARD constexpr scaled_integer<Rep, power<Exponent, Radix>>
    pow(scaled_integer<Rep, power<Exponent, Radix>> const& x) noexcept
    {
        return _impl::crib<Rep, Exponent, Radix, std::pow>(x);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::scaled_integer streaming - (placeholder implementation)

    template<typename Rep, int Exponent, int Radix>
    ::std::ostream& operator<<(::std::ostream& out, scaled_integer<Rep, power<Exponent, Radix>> const& fp)
    {
        return out << to_chars(fp).data();
    }

    template<typename Rep, int Exponent, int Radix>
    ::std::istream& operator>>(::std::istream& in, scaled_integer<Rep, power<Exponent, Radix>>& fp)
    {
        long double ld{};
        in >> ld;
        fp = ld;
        return in;
    }
}

namespace cnl {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits for cnl::scaled_integer

    // note: some members are guessed,
    // some are temporary (assuming rounding style, traps etc.)
    // and some are undefined
    template<typename Rep, int Exponent, int Radix>
    struct numeric_limits<cnl::scaled_integer<Rep, power<Exponent, Radix>>>
            : numeric_limits<cnl::_impl::number<Rep, power<Exponent, Radix>>> {
        // scaled_integer-specific helpers
        using _value_type = cnl::scaled_integer<Rep, power<Exponent, Radix>>;
        using _rep_numeric_limits = numeric_limits<Rep>;

        // standard members

        CNL_NODISCARD static constexpr _value_type min() noexcept
        {
            return _impl::from_rep<_value_type>(Rep{1});
        }

        CNL_NODISCARD static constexpr _value_type max() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::max());
        }

        CNL_NODISCARD static constexpr _value_type lowest() noexcept
        {
            return _impl::from_rep<_value_type>(_rep_numeric_limits::lowest());
        }

        static constexpr bool is_specialized = true;

        static constexpr bool is_integer = false;

        CNL_NODISCARD static constexpr _value_type epsilon() noexcept
        {
            return _impl::from_rep<_value_type>(Rep{1});
        }

        CNL_NODISCARD static constexpr _value_type round_error() noexcept
        {
            return _impl::from_rep<_value_type>(Rep{0});
        }

        CNL_NODISCARD static constexpr _value_type infinity() noexcept
        {
            return _impl::from_rep<_value_type>(Rep{0});
        }

        CNL_NODISCARD static constexpr _value_type quiet_NaN() noexcept
        {
            return _impl::from_rep<_value_type>(Rep{0});
        }

        CNL_NODISCARD static constexpr _value_type signaling_NaN() noexcept
        {
            return _impl::from_rep<_value_type>(Rep{0});
        }

        CNL_NODISCARD static constexpr _value_type denorm_min() noexcept
        {
            return _impl::from_rep<_value_type>(Rep{1});
        }
    };
}

namespace std {
    ////////////////////////////////////////////////////////////////////////////////
    // std::numeric_limits specialization for rounding_integer

    template<typename Rep, class Scale>
    struct numeric_limits<cnl::scaled_integer<Rep, Scale>>
            : cnl::numeric_limits<cnl::scaled_integer<Rep, Scale>> {
    };

    template<typename Rep, class Scale>
    struct numeric_limits<cnl::scaled_integer<Rep, Scale> const>
            : cnl::numeric_limits<cnl::scaled_integer<Rep, Scale>> {
    };
}

#endif  // CNL_IMPL_SCALED_INTEGER_EXTRAS_H
