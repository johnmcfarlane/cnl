
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
#include "../num_traits/width.h"
#include "definition.h"
#include "to_chars.h"

#include <cmath>
#if defined(CNL_IOSTREAMS_ENABLED)
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
    /// \sa `std::abs`

    template<typename Rep, class Scale>
    [[nodiscard]] constexpr auto abs(scaled_integer<Rep, Scale> const& x) noexcept -> decltype(-x)
    {
        return (x >= scaled_integer<Rep, Scale>{}) ? static_cast<decltype(-x)>(x) : -x;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::floor

    template<class Rep, scaled_tag Scale>
    requires(_impl::exponent_v<Scale> < 0)
            [[nodiscard]] constexpr auto floor(scaled_integer<Rep, Scale> const& x)
    {
        static_assert(
                _impl::radix_v<Scale> == 2, "cnl::floor(scaled_integer<Rep, Scale>) not "
                            "implemented for exponent<0 && Radix!=2");

        /// \cond
        return _impl::from_rep<scaled_integer<Rep, typename Scale::identity>>(
                _impl::to_rep(x) >> constant<-_impl::exponent_v<Scale>>());
        /// \endcond
    }

    template<class Rep, scaled_tag Scale>
    requires(_impl::exponent_v<Scale> >= 0)
            [[nodiscard]] constexpr auto floor(scaled_integer<Rep, Scale> const& x)
    {
        return x;
    }

    ////////////////////////////////////////////////////////////////////////////////
    // scaled_integer trig functions
    //
    // Placeholder implementations fall back on <cmath> functions which is slow
    // due to conversion to and from floating-point types; also inconvenient as
    // many <cmath> functions are not [[nodiscard]] constexpr.

    namespace _impl {
        template<int NumBits>
        struct float_of_size;

        template<int NumBits>
        requires(NumBits <= sizeof(float) * CHAR_BIT) struct float_of_size<NumBits> {
            using type = float;
        };

        template<int NumBits>
        requires(sizeof(float) * CHAR_BIT < NumBits && NumBits <= sizeof(double) * CHAR_BIT) struct float_of_size<NumBits> {
            using type = double;
        };

        template<int NumBits>
        requires(sizeof(double) * CHAR_BIT < NumBits && NumBits <= sizeof(long double) * CHAR_BIT) struct float_of_size<NumBits> {
            using type = long double;
        };

        template<class T>
        using float_of_same_size = typename float_of_size<_impl::width<T>>::type;

        template<
                typename Rep, scaled_tag Scale,
                _impl::float_of_same_size<Rep> (*F)(_impl::float_of_same_size<Rep>)>
        [[nodiscard]] constexpr auto
        crib(scaled_integer<Rep, Scale> const& x) noexcept
        {
            using floating_point = _impl::float_of_same_size<Rep>;
            return static_cast<scaled_integer<Rep, Scale>>(
                    F(static_cast<floating_point>(x)));
        }
    }

    template<typename Rep, scaled_tag Scale>
    [[nodiscard]] constexpr auto sin(scaled_integer<Rep, Scale> const& x) noexcept
    {
        return _impl::crib<Rep, Scale, std::sin>(x);
    }

    template<typename Rep, scaled_tag Scale>
    [[nodiscard]] constexpr auto cos(scaled_integer<Rep, Scale> const& x) noexcept
    {
        return _impl::crib<Rep, Scale, std::cos>(x);
    }

    template<typename Rep, scaled_tag Scale>
    [[nodiscard]] constexpr auto exp(scaled_integer<Rep, Scale> const& x) noexcept
    {
        return _impl::crib<Rep, Scale, std::exp>(x);
    }

    template<typename Rep, scaled_tag Scale>
    [[nodiscard]] constexpr auto pow(scaled_integer<Rep, Scale> const& x) noexcept
    {
        return _impl::crib<Rep, Scale, std::pow>(x);
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::scaled_integer streaming - (placeholder implementation)

#if defined(CNL_IOSTREAMS_ENABLED)
    template<typename Rep, scaled_tag Scale>
    auto& operator<<(std::ostream& out, scaled_integer<Rep, Scale> const& fp)
    {
        return out << to_chars_static(fp).chars.data();
    }

    template<typename Rep, scaled_tag Scale>
    auto& operator>>(std::istream& in, scaled_integer<Rep, Scale>& fp)
    {
        long double ld{};
        in >> ld;
        fp = ld;
        return in;
    }
#endif
}

#endif  // CNL_IMPL_SCALED_INTEGER_EXTRAS_H
