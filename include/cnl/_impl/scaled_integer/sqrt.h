
//          Copyright John McFarlane 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief defines sqrt function for scaled_integer

#if !defined(CNL_IMPL_SCALED_INTEGER_SQRT_H)
#define CNL_IMPL_SCALED_INTEGER_SQRT_H

#include "../config.h"
#include "../num_traits/fixed_width_scale.h"
#include "../num_traits/set_width.h"
#include "../num_traits/unwrap.h"
#include "../num_traits/width.h"
#include "../unreachable.h"
#include "type.h"

/// compositional numeric library
namespace cnl {

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::sqrt helper functions

    namespace _impl {
        template<class Rep>
        [[nodiscard]] constexpr auto sqrt_solve3(Rep n, Rep bit, Rep result) -> Rep
        {
            return (bit != Rep{0})
                         ? (n >= result + bit) ? sqrt_solve3<Rep>(
                                   static_cast<Rep>(n - (result + bit)), static_cast<Rep>(bit >> 2),
                                   static_cast<Rep>((result >> 1) + bit))
                                               : sqrt_solve3<Rep>(
                                                       n, static_cast<Rep>(bit >> 2),
                                                       static_cast<Rep>(result >> 1))
                         : result;
        }

        template<int Exponent>
        struct sqrt_solve1 {
            template<class Rep>
            [[nodiscard]] constexpr auto operator()(Rep n) const
            {
                using widened_rep = _impl::set_width_t<Rep, _impl::width<Rep> * 2>;
                return static_cast<Rep>(sqrt_solve3<widened_rep>(
                        _impl::fixed_width_scale<-Exponent>(static_cast<widened_rep>(n)),
                        widened_rep(
                                (widened_rep{1} << ((countr_used(n) + 1 - Exponent) & ~1)) >> 2),
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
    /// \note This function is a placeholder implementation with poor run-time performance
    /// characteristics.
    ///
    /// \sa multiply

    // https://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Binary_numeral_system_.28base_2.29
    template<typename Rep, int Exponent, int Radix>
    [[nodiscard]] constexpr auto sqrt(scaled_integer<Rep, power<Exponent, Radix>> const& x)
            -> scaled_integer<Rep, power<Exponent, Radix>>
    {
        using type = scaled_integer<Rep, power<Exponent, Radix>>;
        return _impl::to_rep(x) < 0
                     ? _impl::unreachable<type>("negative value passed to cnl::sqrt")
                     : type{_impl::from_rep<type>(_impl::sqrt_solve1<Exponent>{}(unwrap(x)))};
    }
}

#endif  // CNL_IMPL_SCALED_INTEGER_SQRT_H
